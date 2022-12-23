#include "akila/core/audio/audio.hpp"
#include <iostream>
#include "akila/core/time/time.hpp"
#include "akila/core/audio/audio_source.hpp"
#include "akila/core/audio/audio_emitter.hpp"

using namespace akila;

ma_context Audio::context{};
ma_resource_manager Audio::resourceManager{};
ma_engine Audio::engine{};
ma_device Audio::device{};

std::array<Audio::KeepedSound, 512> Audio::keepedSounds;
std::array<Audio::InstancedSound, 1024> Audio::instancedSounds;
std::array<Audio::DetachedSound, 32> Audio::detachedSounds;

float Audio::lastCheck = 0;

void dataCallback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount) {
	(void)pInput;
	ma_engine_read_pcm_frames((ma_engine *)pDevice->pUserData, pOutput, frameCount, NULL);
}

Audio::KeepedSound::KeepedSound(): used{false}, index{0}, path{}, maSound{} {

}

Audio::InstancedSound::InstancedSound(): used{false}, index{0}, keepdSoundIndex{0}, maSound{} {

}

Audio::DetachedSound::DetachedSound(): used{false}, maSound{} {

}

///

void Audio::init() {
	ma_resource_manager_config resourceManagerConfig = ma_resource_manager_config_init();
	resourceManagerConfig.decodedFormat = ma_format_f32;
	resourceManagerConfig.decodedChannels = 0;
	resourceManagerConfig.decodedSampleRate = 48000;

	ma_result result = ma_resource_manager_init(&resourceManagerConfig, &resourceManager);
	if(result != MA_SUCCESS) {
		std::cerr << "Failed to initialize resource manager." << std::endl;
		std::exit(EXIT_FAILURE);
	}

	result = ma_context_init(NULL, 0, NULL, &context);
	if(result != MA_SUCCESS) {
		std::cerr << "Failed to initialize context." << std::endl;
		std::exit(EXIT_FAILURE);
	}
	
	ma_device_config deviceConfig;
	ma_engine_config engineConfig;

	deviceConfig = ma_device_config_init(ma_device_type_playback);
	//deviceConfig.playback.pDeviceID = ...; // peripherique par defaut
	deviceConfig.playback.format = resourceManager.config.decodedFormat;
	deviceConfig.playback.channels = 0;
	deviceConfig.sampleRate = resourceManager.config.decodedSampleRate;
	deviceConfig.dataCallback = dataCallback;
	deviceConfig.pUserData = &engine;

	result = ma_device_init(&context, &deviceConfig, &device);
	if(result != MA_SUCCESS) {
		std::cerr << "Failed to initialize device." << std::endl;
		std::exit(EXIT_FAILURE);
	}

	engineConfig = ma_engine_config_init();
	engineConfig.pDevice = &device;
	engineConfig.pResourceManager = &resourceManager;
	engineConfig.noAutoStart = MA_TRUE;

	result = ma_engine_init(&engineConfig, &engine);
	if(result != MA_SUCCESS) {
		std::cerr << "Failed to initialize engine" << std::endl;
		ma_device_uninit(&device);
		std::exit(EXIT_FAILURE);
	}

	result = ma_engine_start(&engine);
	if(result != MA_SUCCESS) {
		std::cerr << "WARNING: Failed to start engine." << std::endl;
		std::exit(EXIT_FAILURE);
	}

	ma_engine_listener_set_position(&engine, 0, 0, 0, 0);

	for(std::size_t i = 0; i < detachedSounds.size(); ++i) {
		detachedSounds[i].used = false;
	}

	for(std::size_t i = 0; i < instancedSounds.size(); ++i) {
		instancedSounds[i].used = false;
	}

	for(std::size_t i = 0; i < keepedSounds.size(); ++i) {
		keepedSounds[i].used = false;
	}
}

void Audio::terminate() {
	for(std::size_t i = 0; i < detachedSounds.size(); ++i) {
		DetachedSound &sound = detachedSounds[i];
		if(!sound.used) continue;
		ma_sound_stop(&sound.maSound);
		ma_sound_uninit(&sound.maSound);
	}

	for(std::size_t i = 0; i < instancedSounds.size(); ++i) {
		InstancedSound &sound = instancedSounds[i];
		if(!sound.used) continue;
		ma_sound_stop(&sound.maSound);
		ma_sound_uninit(&sound.maSound);
	}

	for(std::size_t i = 0; i < keepedSounds.size(); ++i) {
		KeepedSound &sound = keepedSounds[i];
		if(!sound.used) continue;
		ma_sound_stop(&sound.maSound);
		ma_sound_uninit(&sound.maSound);
	}

	ma_engine_stop(&engine);
	ma_engine_uninit(&engine);
	ma_device_uninit(&device);
	ma_context_uninit(&context);
	ma_resource_manager_uninit(&resourceManager);
}

Audio::KeepedSound *Audio::createKeepedSound(std::string const &path) {
	KeepedSound *sound = nullptr;

	std::size_t i = 0;
	for(; i < keepedSounds.size(); ++i) {
		KeepedSound *s = &keepedSounds[i];
		if(s->used == false) {
			sound = s;
			break;
		}
	}

	if(sound == nullptr) return nullptr;

	sound->used = true;
	sound->index = i;
	sound->path = path;

	ma_uint32 const flags =
		MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE |
		MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_ASYNC |
		MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_STREAM;

	ma_result const result = ma_sound_init_from_file(
		&Audio::engine, path.c_str(),
		flags, NULL, NULL, &sound->maSound
	);

	if(result != MA_SUCCESS) {
		std::cerr << "Can't init " << path << std::endl;
	}

	ma_sound_set_spatialization_enabled(&sound->maSound, false);

	return sound;
}

void Audio::eraseKeepedSound(KeepedSound *sound) {
	if(sound == nullptr) return;

	checkDetachedSounds();

	ma_sound_stop(&sound->maSound);
	ma_sound_uninit(&sound->maSound);

	sound->used = false;
}

Audio::InstancedSound *Audio::createInstancedSound(KeepedSound *keepedSound) {
	InstancedSound *sound = nullptr;

	std::size_t i = 0;
	for(; i < instancedSounds.size(); ++i) {
		InstancedSound *s = &instancedSounds[i];
		if(s->used == false) {
			sound = s;
			break;
		}
	}

	if(sound == nullptr) return nullptr;

	sound->used = true;
	sound->index = i;
	sound->keepdSoundIndex = keepedSound->index;

	ma_uint32 const flags = MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE;
	ma_result const result = ma_sound_init_from_file(
		&Audio::engine, keepedSound->path.c_str(),
		flags, NULL, NULL, &sound->maSound
	);

	if(result != MA_SUCCESS) {
		std::cerr << "Can't init " << keepedSound->path << std::endl;
	}

	ma_sound_set_spatialization_enabled(&sound->maSound, true);

	return sound;
}

void Audio::eraseInstancedSound(InstancedSound *sound) {
	if(sound == nullptr) return;

	ma_sound_stop(&sound->maSound);
	ma_sound_uninit(&sound->maSound);

	sound->used = false;
}

Audio::DetachedSound *Audio::createDetachedSound(KeepedSound *keepedSound) {
	DetachedSound *sound = nullptr;

	std::size_t i = 0;
	for(; i < detachedSounds.size(); ++i) {
		DetachedSound *s = &detachedSounds[i];
		if(s->used == false) {
			sound = s;
			break;
		}
	}

	if(sound == nullptr) return nullptr;

	sound->used = true;

	ma_uint32 const flags = MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE;
	ma_result const result = ma_sound_init_from_file(
		&Audio::engine, keepedSound->path.c_str(),
		flags, NULL, NULL, &sound->maSound
	);

	return sound;
}

void Audio::detach(AudioSource const *source) {
	checkDetachedSounds();
	DetachedSound *sound = createDetachedSound(source->sound);
	if(sound == nullptr) return;
	ma_sound_start(&sound->maSound);
}

void Audio::detach(AudioEmitter const *emitter) {
	checkDetachedSounds();
	DetachedSound *sound = createDetachedSound(emitter->source->sound);
	if(sound == nullptr) return;

	ma_vec3f pos = ma_sound_get_position(&emitter->sound->maSound);
	ma_sound_set_position(&sound->maSound, pos.x, pos.y, pos.z);

	//ma_sound_set_max_distance(sound, 100);
	//ma_sound_set_attenuation_model(sound, 10);

	ma_sound_start(&sound->maSound);
}

void Audio::detach(class AudioEmitter const &emitter) {
	detach(&emitter);
}

void Audio::setListenerPositionDirection(Vec3 const &pos, Vec3 const &dir) {
	ma_engine_listener_set_position(&engine, 0, pos.x, pos.y, pos.z);
	ma_engine_listener_set_direction(&engine, 0, dir.x, dir.y, dir.z);
}

void Audio::checkDetachedSounds() {
	float const now = Time::now;
	float const delta = .1f;
	if(lastCheck + delta > now) return;

	lastCheck = now;

	for(std::size_t i = 0; i < detachedSounds.size(); ++i) {
		DetachedSound &sound = detachedSounds[i];

		if(!sound.used) continue;

		if(ma_sound_at_end(&sound.maSound)) {
			ma_sound_uninit(&sound.maSound);
			sound.used = false;
		}
	}
}
