#include "akila/core/inputs/mouse.hpp"

using namespace akila;

Mouse::Mouse(): Controller{-2} {
	setName("Mouse");
	setGuid(Controller::MOUSE_GUID);

	reset();
}

void Mouse::reset() {
	for(int i = 0; i < buttons.size(); ++i) {
		buttons[i] = false;
	}
}

void Mouse::update() {

}
