#include "akila/default/systems/orbit_camera_system.hpp"
#include "akila/default/components/camera_component.hpp"

#include "akila/akila.hpp"

using namespace akila;

OrbitCameraSystem::OrbitCameraSystem(): System{ECS::createSignature<OrbitCameraComponent>()} {};

void OrbitCameraSystem::update() {
	if(entities.empty()) return;

	auto &cam = entities[0].getComponent<OrbitCameraComponent>();

	Vec3 &eye = cam.position;

	if(Inputs::isPressed(Inputs::Key::LEFT_CLICK)) {
		Vec2 &d = Inputs::getMouseVelocity();

		cam.movement.x -= d.x * 0.006;
		cam.movement.y -= d.y * 0.006;
	}

	cam.movement.z -= Inputs::getMouseScrollVelocity().y;

	float dx = 0;
	float dy = 0;
	float dz = 0;

	if(Inputs::isPressed(Inputs::Key::RIGHT_CLICK)) {
		Vec2 &d = Inputs::getMouseVelocity();

		float speedCurve = 0.005 * (0.227297 * cam.distance + 0.533723);
		dx = -d.x * speedCurve;
		dz = -d.y * speedCurve;
	}

	if(Inputs::isPressed(Inputs::Key::MIDDLE_CLICK)) {
		float speedCurve = 0.005 * (0.227297 * cam.distance + 0.533723);
		dy = Inputs::getMouseVelocity().y * speedCurve;
	}

	cam.center.x += cam.view[0][0] * dx + cam.view[0][1] * -dz + cam.view[0][2] * dz;
	cam.center.y += dy;
	cam.center.z += cam.view[2][0] * dx + cam.view[2][1] * -dz + cam.view[2][2] * dz;

	///////////////////////////////////////////////////////////

	cam.distance += cam.movement.z;
	if(cam.distance < 1) cam.distance = 1;
	else if(cam.distance > 100) cam.distance = 100;

	float angle = atan2(cam.movement.x, cam.movement.y);
	float length = sqrt(pow(cam.movement.x, 2) + pow(cam.movement.y, 2));

	cam.angles.x -= cos(angle) * length;
	cam.angles.y -= sin(angle) * length;

	float cax = cos(cam.angles.x);
	float cay = cos(cam.angles.y);
	float say = sin(cam.angles.y);

	eye.x = (cay * cax) * cam.distance + cam.center.x;
	eye.y = (sin(cam.angles.x)) * cam.distance + cam.center.y;
	eye.z = (say * cax) * cam.distance + cam.center.z;


	float angle2 = cam.angles.x + HALF_PI;
	float cangle2 = cos(angle2);
	cam.up.x = cay * cangle2;
	cam.up.y = sin(angle2);
	cam.up.z = say * cangle2;

	cam.view = lookAt(eye, cam.center, cam.up);
	cam.pv = cam.projection * cam.view;

	cam.movement = Vec3{0.f, 0.f, 0.f};
}

Entity OrbitCameraSystem::getMainCam() {
	return entities[0];
}
