#include "akila/core/audio/audio_emitter.hpp"
#include "akila/core/audio/audio.hpp"

#include <iostream>

using namespace akila;

AudioEmitter::AudioEmitter(): sound{nullptr} {

}

AudioEmitter::AudioEmitter(Ref<AudioSource> source): source{source}, sound{nullptr} {
	sound = Audio::createInstancedSound(source->sound);
}

AudioEmitter::~AudioEmitter() {
	Audio::eraseInstancedSound(sound);
}

AudioEmitter::AudioEmitter(AudioEmitter const &other): source{other.source}, sound{nullptr} {
	sound = Audio::createInstancedSound(source->sound);
}

AudioEmitter &AudioEmitter::operator=(AudioEmitter const &other) {
	source = other.source;
	sound = Audio::createInstancedSound(source->sound);

	return *this;
}

AudioEmitter::AudioEmitter(AudioEmitter &&other) noexcept: source{other.source}, sound{other.sound} {
	other.sound = nullptr;
}

AudioEmitter &AudioEmitter::operator=(AudioEmitter &&other) noexcept {
	source = other.source;
	sound = other.sound;
	other.sound = nullptr;

	return *this;
}

void AudioEmitter::setSource(Ref<AudioSource> s) {
	Audio::eraseInstancedSound(sound);
	source = s;
	sound = Audio::createInstancedSound(source->sound);
}
