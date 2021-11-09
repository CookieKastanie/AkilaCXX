#include "Akila/inputs/Mouse.hpp"

using namespace Akila;

Mouse::Mouse(): position{0.f, 0.f}, velocity{0.f, 0.f}, scroll{0.f, 0.f} {
	for(int i = 0; i < KEY_COUNT; ++i) {
		keysState[i] = false;
	}

	detachPressEvent();
}

glm::vec2 &Mouse::getPosition() {
	return position;
}

glm::vec2 &Mouse::getVelocity() {
	return velocity;
}

glm::vec2 &Mouse::getScroll() {
	return scroll;
}

bool Mouse::isPressed(Key k) const {
	return keysState[static_cast<int>(k)];
}

void Mouse::setPosition(float x, float y) {
	velocity.x = x - position.x;
	velocity.y = position.y - y;

	position.x = x;
	position.y = y;
}

void Mouse::setScrollOffset(float x, float y) {
	scroll.x = x;
	scroll.y = y;
}

void Mouse::update() {
	velocity.x = 0;
	velocity.y = 0;

	scroll.x = 0;
	scroll.y = 0;
}

void Mouse::setKeyState(Key k, bool state) {
	keysState[static_cast<int>(k)] = state;
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

