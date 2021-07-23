#pragma once

#include "Akila/graphics/Camera.hpp"
#include "Akila/inputs/Mouse.hpp"

#define HALF_PI 1.57079632679

class MouseCamera: public Akila::PerspectiveCamera {
private:
	Akila::Mouse *mouse;

	glm::vec3 up;
	glm::vec3 center;

	glm::vec3 angles;
	glm::vec3 movement;
	float distance;

public:
	MouseCamera(Akila::Mouse *mouse);

	void update() override;
};
