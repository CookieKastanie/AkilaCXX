#include "akila/engine/systems/free_camera_system.hpp"
#include "akila/core/inputs/inputs.hpp"
#include "akila/core/math/math.hpp"

using namespace akila;

FreeCameraSystem::FreeCameraSystem():
	CameraSystem{},
	camera{} {
	camera.resize(Window::getSize());
}

void FreeCameraSystem::update() {
	float &distance = camera.distance;
	float &maxDistance = camera.maxDistance;
	Vec3 &center = camera.center;
	Vec3 &angles = camera.angles;
	Vec3 &position = camera.position;

	Mat4 &view = camera.view;
	Mat4 &projection = camera.projection;
	Mat4 &pv = camera.pv;

	//

	Vec2 angularDelta = {0, 0};
	Vec3 translateDelta = {0, 0, 0};

	if(Inputs::isPressed(Inputs::Key::LEFT_CLICK)) {
		angularDelta -= Inputs::getMouseVelocity() * 0.006f;
	}

	distance -= Inputs::getMouseScrollVelocity().y;
	if(distance < 1) distance = 1;
	else if(distance > maxDistance) distance = maxDistance;

	if(Inputs::isPressed(Inputs::Key::RIGHT_CLICK)) {
		float speedCurve = 0.005f * (0.227297f * distance + 0.533723f);
		Vec2 d = Inputs::getMouseVelocity() * speedCurve;

		translateDelta.x = -d.x;
		translateDelta.z = -d.y;
	}

	if(Inputs::isPressed(Inputs::Key::MIDDLE_CLICK)) {
		float speedCurve = 0.005f * (0.227297f * distance + 0.533723f);
		translateDelta.y = Inputs::getMouseVelocity().y * speedCurve;
	}

	center.x += view[0][0] * translateDelta.x + view[0][1] * -translateDelta.z + view[0][2] * translateDelta.z;
	center.y += translateDelta.y;
	center.z += view[2][0] * translateDelta.x + view[2][1] * -translateDelta.z + view[2][2] * translateDelta.z;

	float angle = atan2(angularDelta.x, angularDelta.y);
	float length = hypot(angularDelta.x, angularDelta.y);

	angles.x -= cos(angle) * length;
	angles.y -= sin(angle) * length;

	float cax = cos(angles.x);
	float cay = cos(angles.y);
	float say = sin(angles.y);

	position.x = (cay * cax) * distance + center.x;
	position.y = (sin(angles.x)) * distance + center.y;
	position.z = (say * cax) * distance + center.z;

	float angle2 = angles.x + HALF_PI;
	float cangle2 = cos(angle2);

	Vec3 up{
		cay * cangle2,
		sin(angle2),
		say * cangle2
	};

	view = lookAt(position, center, up);
	pv = projection * view;
}

void FreeCameraSystem::onResize(IVec2 const &size) {
	camera.resize(size);
}

CameraData const *FreeCameraSystem::getCameraData() {
	return camera.getData();
}
