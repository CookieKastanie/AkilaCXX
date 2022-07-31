#include "akila/audio/audio_source.hpp"
#include "akila/audio/audio.hpp"

using namespace akila;

AudioSource::AudioSource(): sound{} {
}

AudioSource::~AudioSource() {
	//ma_resource_manager_data_source_uninit(&sound);
	ma_sound_uninit(&sound);
}

bool AudioSource::loadFromFile(std::string const &path) {
	this->path = path;
	ma_result result = ma_sound_init_from_file(&Audio::engine, path.c_str(), 0, NULL, NULL, &sound);

	/*
	ma_result result = ma_resource_manager_data_source_init(
		ma_engine_get_resource_manager(&Audio::engine),
		path.c_str(),
		MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE,
		NULL,
		&source
	);
	//*/

	return result == MA_SUCCESS;
}

void AudioSource::play() {
	ma_engine_play_sound(&Audio::engine, path.c_str(), NULL);
}
