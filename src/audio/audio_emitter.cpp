#include "akila/audio/audio_emitter.hpp"
#include "akila/audio/audio.hpp"

#include <iostream>

#include "akila/akila.hpp"

using namespace akila;

AudioEmitter::AudioEmitter(): sound{} {

}

AudioEmitter::AudioEmitter(Ref<AudioSource> source): source{source}, sound{}{
	source->initSound(&sound);
}

AudioEmitter::~AudioEmitter() {
	ma_sound_stop(&sound);
	ma_sound_uninit(&sound);
}


AudioEmitter::AudioEmitter(AudioEmitter const &other): source{other.source}, sound{} {
	source->initSound(&sound);
}

AudioEmitter &AudioEmitter::operator=(AudioEmitter const &other) {
	source = other.source;
	source->initSound(&sound);

	return *this;
}

void AudioEmitter::setSource(Ref<AudioSource> s) {
	source = s;
	source->initSound(&sound);
}

void AudioEmitter::play() {
	ma_sound_start(&sound);
}

bool AudioEmitter::isFinished() {
	return ma_sound_at_end(&sound) == MA_TRUE;
}

void AudioEmitter::setPosition(Vec3 const &p) {
	ma_sound_set_position(&sound, p.x, p.y, p.z);
}
