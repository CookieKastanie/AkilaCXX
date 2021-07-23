#include "Demo/MouseCamera.hpp"

MouseCamera::MouseCamera(Akila::Mouse *mouse):
	mouse{mouse},
	up{0., 0., 0.},
	center{0., 0., 0.},
	angles{0., 0., 0.},
	movement{0., 0., 0.},
	distance{3.} {

}

void MouseCamera::update() {
	glm::vec3 &eye = uniforms.position;

	if(mouse->isPressed(Akila::Mouse::Key::LEFT)) {
		glm::vec2 &d = mouse->getVelocity();

		movement[0] -= d[0] * 0.005;
		movement[1] += d[1] * 0.005;
	}

	movement[2] -= mouse->getScroll()[1];

	float dx = 0;
	float dy = 0;
	float dz = 0;

	if(mouse->isPressed(Akila::Mouse::Key::RIGHT)) {
		glm::vec2 &d = mouse->getVelocity();
		dx = -d[0] * 0.005;
		dz = d[1] * 0.005;
	}

	if(mouse->isPressed(Akila::Mouse::Key::MIDDLE)) {
		dy = -mouse->getVelocity()[1] * 0.005;
	}

	center[0] += uniforms.view[0][0] * dx + uniforms.view[0][1] * -dz + uniforms.view[0][2] * dz;
	center[1] += dy;
	center[2] += uniforms.view[2][0] * dx + uniforms.view[2][1] * -dz + uniforms.view[2][2] * dz;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	distance += movement[2];
	if(distance < .01) distance = .01;
	else if(distance > 100) distance = 100;

	const float angle = atan2(movement[0], movement[1]);
	const float length = sqrt(pow(movement[0], 2) + pow(movement[1], 2));

	angles[0] -= cos(angle) * length;
	angles[1] -= sin(angle) * length;

	const float cax = cos(angles[0]);
	const float cay = cos(angles[1]);
	const float say = sin(angles[1]);

	eye[0] = (cay * cax) * distance + center[0];
	eye[1] = (sin(angles[0])) * distance + center[1];
	eye[2] = (say * cax) * distance + center[2];


	const float angle2 = angles[0] + HALF_PI;
	const float cangle2 = cos(angle2);
	up[0] = cay * cangle2;
	up[1] = sin(angle2);
	up[2] = say * cangle2;

	uniforms.view = glm::lookAt(eye, center, up);
	uniforms.pv = uniforms.projection * uniforms.view;

	movement[0] = 0;
	movement[1] = 0;
	movement[2] = 0;
}