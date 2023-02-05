#include "akila/engine/systems/audio_system.hpp"

using namespace akila;

AudioSystem::AudioSystem():
	System{ECS::createSignature<TransformComponent, AudioComponent>()},
	lastCameraPosition{0.f, 0.f, 0.f} {

}

void AudioSystem::updateTick() {
	for(Entity e : entities) {
		auto &a = e.getComponent<AudioComponent>();
		if(a.isPlaying() == false) {
			continue;
		}

		auto &t = e.getComponent<TransformComponent>();

		Mat4 const &world = t.getWorldMatrix();

		Vec3 const worldPosition = world * Vec4{0.f, 0.f, 0.f, 1.f};
		Vec3 const velocity = worldPosition - a.lastWorldPosition;
		a.lastWorldPosition = worldPosition;

		a.emitter.setPosition(worldPosition);
		a.emitter.setDirection(world * FORWARD_VECTOR);
		a.emitter.setVelocity(velocity);
	}
}

void AudioSystem::updateFrame(CameraData const *camera) {
	Mat4 const &camMat = inverse(camera->view);
	Vec3 const velocity = camera->position - lastCameraPosition;
	lastCameraPosition = camera->position;
	Audio::setListenerAttitude(camera->position, camMat * FORWARD_VECTOR, camMat * UP_VECTOR);
	Audio::setListenerVelocity(velocity);
}

void AudioSystem::detach(Entity e) {
	if(e.hasComponent<AudioComponent>() == false) return;
	if(e.hasComponent<TransformComponent>() == false) return;

	auto &a = e.getComponent<AudioComponent>();
	auto &t = e.getComponent<TransformComponent>();

	Mat4 const &world = t.getWorldMatrix();
	a.emitter.setPosition(world * Vec4{0.f, 0.f, 0.f, 1.f});
	a.emitter.setDirection(world * FORWARD_VECTOR);

	Audio::detach(a.emitter);
}
