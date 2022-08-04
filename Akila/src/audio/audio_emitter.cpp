#include "akila/audio/audio_emitter.hpp"
#include "akila/audio/audio.hpp"

#include <iostream>

#include "akila/akila.hpp"

using namespace akila;

AudioEmitter::AudioEmitter(): sound{new ma_sound{}} {
	std::cout << "tiens tiens tiens" << std::endl;
}

AudioEmitter::AudioEmitter(Ref<AudioSource> source): source{source}, sound{new ma_sound{}}{
	source->initSound(sound);
	std::cout << "nice" << std::endl;
}

AudioEmitter::~AudioEmitter() {
	if(sound == nullptr) return;

	ma_sound_uninit(sound);
	delete sound;
}

/// LA GROSSE FUITE 
AudioEmitter::AudioEmitter(AudioEmitter const &other): source{other.source}, sound{new ma_sound{}} {
	source->initSound(sound);
}

AudioEmitter &AudioEmitter::operator=(AudioEmitter &&other) noexcept {
	other.sound = sound;
	other.source = source;

	sound = nullptr;

	return *this;
}

void AudioEmitter::setSource(Ref<AudioSource> s) {
	source = s;
	source->initSound(sound);
}

void AudioEmitter::play() {
	ma_sound_start(sound);
}

bool AudioEmitter::isFinished() {
	return ma_sound_at_end(sound) == MA_TRUE;
}

void AudioEmitter::setPosition(Vec3 const &p) {
	ma_sound_set_position(sound, p.x, p.y, p.z);
}
