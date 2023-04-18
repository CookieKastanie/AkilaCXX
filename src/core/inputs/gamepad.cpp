#include "akila/core/inputs/gamepad.hpp"

using namespace akila;

Gamepad::Gamepad(int jid, int index): Controller{jid} {
	realGuid = glfwGetJoystickGUID(jid);
	setName(glfwGetJoystickName(jid));
	assignIndex(index);

	reset();
}

void Gamepad::assignIndex(int i) {
	index = i;
	Controller::Guid guid = Controller::GAMEPAD_GUID_BASE + std::to_string(i);
	setGuid(guid);
}

void Gamepad::setDeadZone(unsigned int index, float deadZone) {
	if(deadZone < 0.f) deadZone = 0.f;
	if(deadZone > 0.999f) deadZone = 0.999f;

	axisDeadZones[index] = deadZone;
}

void Gamepad::reset() {
	for(int i = 0; i <= GLFW_GAMEPAD_BUTTON_LAST; ++i) {
		state.buttons[i] = false;
	}

	for(int i = 0; i <= GLFW_GAMEPAD_AXIS_LAST; ++i) {
		state.axes[i] = 0.f;
		axisDeadZones[i] = 0.1f;
	}

	axisDeadZones[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] = 0.f;
	axisDeadZones[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] = 0.f;
}

void Gamepad::update() {
	if(glfwGetGamepadState(jid, &state) == GLFW_FALSE) {
		return;
	}

	for(int i = 0; i <= GLFW_GAMEPAD_AXIS_LAST; ++i) {
		state.axes[i] = mapToDeadZone(state.axes[i], axisDeadZones[i]);
	}
}
