#include "Akila/inputs/Keyboard.hpp"

using namespace Akila;

Keyboard::Keyboard() {
	for(int i = 0; i < KEY_COUNT; ++i) {
		keysState[i] = false;
	}

	detachPressEvent();
}

bool Keyboard::isPressed(Key k) const {
	return keysState[k];
}

void Keyboard::setKeyState(Key k, bool state) {
	keysState[k] = state;
}

void Keyboard::onKeyPress(const std::function<void(Keyboard*)> &cb) {
	pressCallback = cb;
}

void Keyboard::detachPressEvent() {
	pressCallback = [](Keyboard*) -> void {};
}

void Keyboard::firePressEvent() {
	pressCallback(this);
}