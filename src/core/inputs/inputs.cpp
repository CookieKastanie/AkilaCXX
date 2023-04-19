#include "akila/core/inputs/inputs.hpp"

using namespace akila;

Keyboard Inputs::keyboard{};
Mouse Inputs::mouse{};
std::unordered_map<int, RefAnchor<Gamepad>> Inputs::gamepads;
std::unordered_map<Controller::Guid, RefAnchor<Joystick>> Inputs::joysticks;

std::vector<Controller *> Inputs::controllerAlias;

bool Inputs::gamepadAsJoystick = false;

void Inputs::init(InitValues const &initVals) {
	gamepadAsJoystick = initVals.gamepadAsJoystick;

	keyboard.reset();
	mouse.reset();

	controllerAlias.push_back(&keyboard);
	controllerAlias.push_back(&mouse);

	//check joysticks
	for(int i = 0; i < GLFW_JOYSTICK_LAST; ++i) {
		if(glfwJoystickPresent(i)) {
			addController(i);
		}
	}
}

void Inputs::terminate() {
	gamepads.clear();
	joysticks.clear();
	controllerAlias.clear();
}

bool Inputs::addController(int id) {
	bool isGamepad = (gamepadAsJoystick == false) && glfwJoystickIsGamepad(id);

	if(isGamepad) {
		//find gamepad index
		int gamepadIndex = 1;
		for(int i = 0; i < GLFW_JOYSTICK_LAST; ++i) {
			if(gamepads.find(gamepadIndex) == gamepads.end()) {
				break;
			}

			++gamepadIndex;
		}

		if(gamepadIndex == GLFW_JOYSTICK_LAST) {
			return false;
		}

		Gamepad *gamepad = new Gamepad{id, gamepadIndex};
		gamepads[gamepadIndex].setValue(gamepad);

		controllerAlias.push_back(gamepad);
	} else {
		Joystick *joystick = new Joystick{id};

		if(joysticks.find(joystick->getGuid()) != joysticks.end()) {
			// reconnect
		}

		joysticks[joystick->getGuid()].setValue(joystick);

		controllerAlias.push_back(joystick);
	}
	
	return true;
}

void Inputs::removeController(int id) {
	Controller *controllerToRemove = nullptr;

	for(auto &[index, gamepad] : gamepads) {
		if(gamepad.raw()->getJid() == id) {
			controllerToRemove = gamepad.raw();

			if(gamepad.haveReferences() == false) {
				gamepads.erase(index);
			}

			break;
		}
	}

	for(auto &[guid, joystick] : joysticks) {
		if(joystick.raw()->getJid() == id) {
			controllerToRemove = joystick.raw();
			
			if(joystick.haveReferences() == false) {
				joysticks.erase(guid);
			}

			break;
		}
	}

	auto it = std::find(controllerAlias.begin(), controllerAlias.end(), controllerToRemove);
	if(it != controllerAlias.end()) {
		controllerAlias.erase(it);
	}
}

void Inputs::updateControllers() {
	// skip mouse and keyboard
	for(int i = 2; i < controllerAlias.size(); ++i) {
		controllerAlias[i]->update();
	}
}

void Inputs::updateCurrentBindings() {
	//bindings.update();
}
