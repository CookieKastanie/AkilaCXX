#include "akila/core/audio/audio_source.hpp"
#include "akila/core/audio/audio.hpp"

using namespace akila;

AudioSource::AudioSource(): sound{nullptr} {}

AudioSource::~AudioSource() {
	Audio::eraseKeepedSound(sound);
}

bool AudioSource::decodeFile(std::string const &p) {
	if(sound != nullptr) {
		std::cerr << p << " already decoded" << std::endl;
		return false;
	}

	sound = Audio::createKeepedSound(p);

	return sound != nullptr;
}

bool AudioSource::play() {
	return ma_sound_start(&sound->maSound) == MA_SUCCESS;
}

bool AudioSource::stop() {
	return ma_sound_stop(&sound->maSound) == MA_SUCCESS;
}

bool AudioSource::isFinished() {
	return ma_sound_at_end(&sound->maSound) == MA_TRUE;
}

void AudioSource::setVolume(float volume) {
	ma_sound_set_volume(&sound->maSound, volume);
}

void AudioSource::setLooping(bool loop) {
	ma_sound_set_looping(&sound->maSound, loop);
}
