#include "akila/engine/systems/audio_system.hpp"

using namespace akila;

AudioSystem::AudioSystem(): System{ECS::createSignature<TransformComponent, AudioComponent>()} {

}

void AudioSystem::updateTick() {
	for(Entity e : entities) {
		auto &a = e.getComponent<AudioComponent>();
		if(a.isPlaying() == false) {
			continue;
		}

		auto &t = e.getComponent<TransformComponent>();

		Mat4 const &world = t.getWorldMatrix();
		a.emitter.setPosition(world * Vec4{0.f, 0.f, 0.f, 1.f});
		a.emitter.setDirection(world * FORWARD_VECTOR);
	}
}

void AudioSystem::updateFrame(CameraData const *camera) {
	Mat4 const &camMat = inverse(camera->view);
	Audio::setListenerAttitude(camera->position, camMat * FORWARD_VECTOR, camMat * UP_VECTOR);
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
