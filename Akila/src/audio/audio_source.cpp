#include "akila/audio/audio_source.hpp"
#include "akila/audio/audio.hpp"

using namespace akila;

AudioSource::AudioSource(): sound{} {
}

AudioSource::~AudioSource() {
	ma_sound_uninit(&sound);
}

bool AudioSource::loadFromFile(std::string const &path) {
	ma_result result = ma_sound_init_from_file(
		&Audio::engine,
		path.c_str(),
		MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE | MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_ASYNC | MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_STREAM,
		NULL,
		NULL,
		&sound
	);

	return result == MA_SUCCESS;
}

bool AudioSource::play() {
	return ma_sound_start(&sound) == MA_SUCCESS;
}
