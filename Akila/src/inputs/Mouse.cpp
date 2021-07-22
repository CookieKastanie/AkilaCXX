#include "Akila/inputs/Mouse.hpp"

using namespace Akila;

Mouse::Mouse() {
	for(int i = 0; i < KEY_COUNT; ++i) {
		keysState[i] = false;
	}

	detachPressEvent();
}

glm::vec2 &Mouse::getPosition() {
	return position;
}

bool Mouse::isPressed(Key k) const {
	return keysState[k];
}

void Mouse::setPosition(float x, float y) {
	position.x = x;
	position.y = y;
}

void Mouse::setKeyState(Key k, bool state) {
	keysState[k] = state;
}

void Mouse::onKeyPress(const std::function<void(Key)> &cb) {
	pressCallback = cb;
}

void Mouse::detachPressEvent() {
	pressCallback = [](Key) -> void {};
}

void Mouse::firePressEvent(Key &key) {
	pressCallback(key);
}

