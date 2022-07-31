#include "akila/audio/audio_emitter.hpp"
#include "akila/audio/audio.hpp"

using namespace akila;

AudioEmitter::AudioEmitter(AudioBuffer &buffer): sound{} {
	ma_sound_init_from_data_source(&Audio::engine, &buffer.source, NULL, NULL, &sound);
}

AudioEmitter::AudioEmitter(AudioEmitter &audioEmmiter): sound{} {
	ma_sound_init_copy(&Audio::engine, &audioEmmiter.sound, NULL, NULL, &sound);
}

AudioEmitter::~AudioEmitter() {
	ma_sound_uninit(&sound);
}

void AudioEmitter::play() {
	ma_sound_start(&sound);
}
