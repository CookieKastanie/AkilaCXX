#include "akila/audio/audio.hpp"
#include <iostream>
#include "akila/time/time.hpp"

using namespace akila;

ma_context Audio::context;
ma_resource_manager Audio::resourceManager;
ma_engine Audio::engine;
ma_device Audio::device;

std::deque<ma_sound> Audio::detachedSounds;

void dataCallback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount) {
	(void)pInput;
	ma_engine_read_pcm_frames((ma_engine *)pDevice->pUserData, pOutput, frameCount, NULL);
}

void Audio::init() {
	ma_resource_manager_config resourceManagerConfig = ma_resource_manager_config_init();
	resourceManagerConfig.decodedFormat = ma_format_f32;
	resourceManagerConfig.decodedChannels = 0;
	resourceManagerConfig.decodedSampleRate = 48000;

	ma_result result = ma_resource_manager_init(&resourceManagerConfig, &resourceManager);
	if(result != MA_SUCCESS) {
		std::cerr << "Failed to initialize resource manager." << std::endl;
		return std::exit(1);
	}

	result = ma_context_init(NULL, 0, NULL, &context);
	if(result != MA_SUCCESS) {
		std::cerr << "Failed to initialize context." << std::endl;
		return std::exit(1);
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
		std::exit(1);
	}

	engineConfig = ma_engine_config_init();
	engineConfig.pDevice = &device;
	engineConfig.pResourceManager = &resourceManager;
	engineConfig.noAutoStart = MA_TRUE;

	result = ma_engine_init(&engineConfig, &engine);
	if(result != MA_SUCCESS) {
		std::cerr << "Failed to initialize engine" << std::endl;
		ma_device_uninit(&device);
		std::exit(1);
	}

	result = ma_engine_start(&engine);
		if(result != MA_SUCCESS) {
		std::cerr << "WARNING: Failed to start engine." << std::endl;
	}

		ma_engine_listener_set_position(&engine, 0, 0, 0, 0);
}

void Audio::terminate() {
	ma_engine_uninit(&engine);
	ma_device_uninit(&device);
	ma_resource_manager_uninit(&resourceManager);
}

void Audio::detach(AudioSource const *source) {
	checkDetechedSounds();

	detachedSounds.emplace_back();
	ma_sound *sound = &detachedSounds.back();

	source->initSound(sound);

	ma_sound_start(sound);
}

void Audio::detach(AudioEmitter const *emitter) {
	checkDetechedSounds();

	detachedSounds.emplace_back(emitter->sound);
	ma_sound *sound = &detachedSounds.back();

	emitter->source->initSound(sound);

	ma_vec3f pos = ma_sound_get_position(&emitter->sound);
	ma_sound_set_position(sound, pos.x, pos.y, pos.z);

	//ma_sound_set_max_distance(sound, 100);
	//ma_sound_set_attenuation_model(sound, 10);

	ma_sound_start(sound);
}

void Audio::setListenerPositionDirection(Vec3 const &pos, Vec3 const &dir) {
	//ma_engine_listener_set_position(&engine, 0, pos.x * 0.1, pos.y * 0.1, pos.z * 0.1);
	ma_engine_listener_set_position(&engine, 0, pos.x, pos.y, pos.z);
	ma_engine_listener_set_direction(&engine, 0, dir.x, dir.y, dir.z);
}

float Audio::lastCheck = 0;

void Audio::checkDetechedSounds() {
	float const now = Time::now;
	float const delta = 1;
	if(lastCheck + delta > now) return;

	lastCheck = now;

	for(auto it = detachedSounds.begin(); it != detachedSounds.end();) {
		if(ma_sound_at_end(&*it) == MA_TRUE) {
			ma_sound *sound = &*it;
			ma_sound_uninit(sound);
			it = detachedSounds.erase(it);
		} else {
			++it;
		}
	}
}
