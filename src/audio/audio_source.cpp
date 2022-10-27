#include "akila/audio/audio_source.hpp"
#include "akila/audio/audio.hpp"

using namespace akila;

AudioSource::AudioSource(): sound{} {}

AudioSource::~AudioSource() {
	ma_sound_stop(&sound);
	ma_sound_uninit(&sound);
}

bool AudioSource::decodeFile(std::string const &p) {
	path = p;
	bool r = initSound(&sound);
	ma_sound_set_spatialization_enabled(&sound, false);
	return r;
}

bool AudioSource::play() {
	return ma_sound_start(&sound) == MA_SUCCESS;
}

bool AudioSource::initSound(ma_sound *otherSound) const {
	ma_uint32 const flags =
		MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE |
		MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_ASYNC |
		MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_STREAM;

	ma_result const result = ma_sound_init_from_file(
		&Audio::engine, path.c_str(),
		flags, NULL, NULL, otherSound
	);

	return result == MA_SUCCESS;
}

void AudioSource::setVolume(float volume) {
	ma_sound_set_volume(&sound, volume);
}

void AudioSource::setLooping(bool loop) {
	ma_sound_set_looping(&sound, loop);
}
