#include "akila/inputs/inputs.hpp"

using namespace akila;

std::array<bool, 512> Inputs::keysState;

bool Inputs::isPressed(Key key) {
	return keysState[static_cast<int>(key)];
}

void Inputs::setInputState(Key key, bool state) {
	keysState[static_cast<int>(key)] = state;
}
