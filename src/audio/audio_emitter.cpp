#include "akila/audio/audio_emitter.hpp"
#include "akila/audio/audio.hpp"

#include <iostream>

#include "akila/akila.hpp"

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

void AudioEmitter::play() {
	ma_sound_start(&sound->maSound);
}

bool AudioEmitter::isFinished() {
	return ma_sound_at_end(&sound->maSound) == MA_TRUE;
}

void AudioEmitter::setPosition(Vec3 const &p) {
	ma_sound_set_position(&sound->maSound, p.x, p.y, p.z);
}
