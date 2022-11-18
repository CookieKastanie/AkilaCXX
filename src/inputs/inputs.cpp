#include "akila/inputs/inputs.hpp"

using namespace akila;

std::array<bool, 512> Inputs::keysState;
Vec2 Inputs::mousePosition = {0, 0};
Vec2 Inputs::mouseVelocity = {0, 0};
Vec2 Inputs::mouseScrollVelocity = {0, 0};

void Inputs::init() {
	for(int i = 0; i < keysState.size(); ++i) {
		keysState[i] = false;
	}
}

bool Inputs::isPressed(Key key) {
	return keysState[static_cast<int>(key)];
}

void Inputs::setInputState(Key key, bool state) {
	keysState[static_cast<int>(key)] = state;
}

void Inputs::setMousePosition(Vec2 const &pos) {
	mousePosition = pos;
}

void Inputs::setMouseVelocity(Vec2 const &vel) {
	mouseVelocity = vel;
}

void Inputs::setMouseScrollVelocity(Vec2 const &scrollVel) {
	mouseScrollVelocity = scrollVel;
}

Vec2 const &Inputs::getMousePosition() {
	return mousePosition;
}

Vec2 const &Inputs::getMouseVelocity() {
	return mouseVelocity;
}

Vec2 const &Inputs::getMouseScrollVelocity() {
	return mouseScrollVelocity;
}
