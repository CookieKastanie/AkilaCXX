#include "akila/audio/audio_source.hpp"
#include "akila/audio/audio.hpp"

using namespace akila;

AudioSource::AudioSource(): sound{new ma_sound{}} {}

AudioSource::~AudioSource() {
	ma_sound_uninit(sound);
	delete sound;
}

bool AudioSource::decodeFile(std::string const &p) {
	path = p;
	return initSound(sound);
}

bool AudioSource::play() {
	return ma_sound_start(sound) == MA_SUCCESS;
}

bool AudioSource::initSound(ma_sound *otherSound) {
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
