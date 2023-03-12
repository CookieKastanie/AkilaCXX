#include "akila/engine/layers/scene_layer.hpp"

using namespace akila;

void SceneLayer::onMount() {
	sceneSystem = ECS::createSystem<SceneSystem>();
	renderSystem = ECS::createSystem<DummyRenderSystem>();
	audioSystem = ECS::createSystem<AudioSystem>();
	cameraSystem = ECS::createSystem<DummyCameraSystem>();
}

void SceneLayer::tick() {
	sceneSystem->updateTick();
}

void SceneLayer::frame() {
	sceneSystem->updateFrame();
	cameraSystem->update();
	audioSystem->updateFrame(cameraSystem->getCameraData());
	renderSystem->updateFrame(cameraSystem->getCameraData());
}
