#include "akila/core/inputs/controller.hpp"

using namespace akila;

std::string Controller::getHatPositionString(HatPosition h) {
	if(h == HatPosition::UP) return "UP";
	if(h == HatPosition::RIGHT) return "RIGHT";
	if(h == HatPosition::DOWN) return "DOWN";
	if(h == HatPosition::LEFT) return "LEFT";
	if(h == HatPosition::RIGHT_UP) return "RIGHT_UP";
	if(h == HatPosition::RIGHT_DOWN) return "RIGHT_DOWN";
	if(h == HatPosition::LEFT_UP) return "LEFT_UP";
	if(h == HatPosition::LEFT_DOWN) return "LEFT_DOWN";

	return "CENTERED";
}

Controller::Controller(int jid): jid{jid} {}

void Controller::setGuid(Guid const &id) {
	guid = id;
}

void Controller::setName(std::string const &n) {
	name = n;
}

float Controller::mapToDeadZone(float value, float deadZone) {
	if(value <= -1.f) return -1.f;
	if(value >= 1.f) return 1.f;

	if(value > deadZone) return (value - deadZone) / (1.f - deadZone);
	if(value < -deadZone) return (value + deadZone) / (1.f - deadZone);

	return 0.f;
}
