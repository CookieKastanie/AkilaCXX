#include "akila/core/inputs/joystick.hpp"

using namespace akila;

Joystick::Joystick(int jid): Controller{jid} {
	setGuid(glfwGetJoystickGUID(jid));
	setName(glfwGetJoystickName(jid));
	joystickIsGamepad = glfwJoystickIsGamepad(jid);

	float defaultDeadZone = 0.025f;
	if(joystickIsGamepad) {
		defaultDeadZone = 0.1f;
	}

	for(std::size_t i = 0; i < axisDeadZones.size(); ++i) {
		axisDeadZones[i] = defaultDeadZone;
	}

	reset();
}

void Joystick::setDeadZone(unsigned int index, float deadZone) {
	if(deadZone < 0.f) deadZone = 0.f;
	if(deadZone > 0.999f) deadZone = 0.999f;

	axisDeadZones[index] = deadZone;
}

void Joystick::reset() {
	for(std::size_t i = 0; i < buttons.size(); ++i) {
		buttons[i] = false;
		axes[i] = 0.f;
		hats[i] = HatPosition::CENTERED;
	}
}

void Joystick::update() {
	unsigned char const * buttonStates = glfwGetJoystickButtons(jid, &buttonCount);
	for(int i = 0; i < buttonCount; ++i) {
		buttons[i] = static_cast<bool>(buttonStates[i]);
	}

	float const *axisStates = glfwGetJoystickAxes(jid, &axisCount);
	for(int i = 0; i < axisCount; ++i) {
		axes[i] = mapToDeadZone(axisStates[i], axisDeadZones[i]);
	}

	unsigned char const *hatStates = glfwGetJoystickHats(jid, &hatCount);
	for(int i = 0; i < buttonCount; ++i) {
		hats[i] = static_cast<HatPosition>(hatStates[i]);
	}
}
