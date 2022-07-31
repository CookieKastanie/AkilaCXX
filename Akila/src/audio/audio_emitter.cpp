#include "akila/audio/audio_emitter.hpp"
#include "akila/audio/audio.hpp"

#include <iostream>

using namespace akila;

AudioEmitter::AudioEmitter(AudioSource &buffer): sound{} {
	//ma_sound_init_from_data_source(&Audio::engine, &buffer.source, NULL, NULL, &sound);
}

AudioEmitter::AudioEmitter(AudioEmitter &audioEmmiter): sound{} {
	//ma_sound_init_copy(&Audio::engine, &audioEmmiter.sound, NULL, NULL, &sound);
}

AudioEmitter::~AudioEmitter() {
	//ma_sound_uninit(&sound);
}

void AudioEmitter::play() {
	//ma_node *pNode = ma_node_graph_get_endpoint(&Audio::engine.nodeGraph);

	//ma_result result = ma_node_attach_output_bus(&sound, 0, pNode, 0);
	//std::cout << result << std::endl;

	//ma_sound_start(&sound);
}

bool AudioEmitter::isFinished() {
	//return ma_sound_at_end(&sound) == MA_TRUE;

	return false;
}
