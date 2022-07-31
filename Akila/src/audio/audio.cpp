#include "akila/audio/audio.hpp"
#include <iostream>

using namespace akila;

ma_engine Audio::engine;

void Audio::init() {
	ma_engine_init(NULL, &engine);
}

void Audio::terminate() {
	ma_engine_uninit(&engine);
}
