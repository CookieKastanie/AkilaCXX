#include "akila/core/inputs/keyboard.hpp"

using namespace akila;

Keyboard::Keyboard(): Controller{-1} {
	setName("Keyboard");
	setGuid(Controller::KEYBOARD_GUID);

	reset();
}

void Keyboard::reset() {
	for(int i = 0; i < buttons.size(); ++i) {
		buttons[i] = false;
	}
}

void Keyboard::update() {

}
