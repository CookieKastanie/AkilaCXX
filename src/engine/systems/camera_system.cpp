#include "akila/engine/systems/camera_system.hpp"
#include "akila/engine/components/camera_component.hpp"
#include "akila/core/window/window.hpp"
#include "akila/core/window/window_events.hpp"

using namespace akila;

CameraSystem::CameraSystem():
	System{ECS::createSignature<CameraComponent>()},
	currentIsEntity{false} {

	std::string cameraName = "free_cam";
	addCameraControler(cameraName, createPtr<FreeCameraControler>());
	setCurrentCamera(cameraName);

	resizeListener = Signals::listen<WindowResizeSignal>([this](WindowResizeSignal const &s) {
		if(currentIsEntity) {

		} else {
			currentCameraControler->resize(s.size);
		}
	});
}

void CameraSystem::addCameraControler(std::string const &name, Ptr<CameraControler> camera) {
	cameraControlers[name] = camera;
}

void CameraSystem::setCurrentCamera(std::string const &name) {
	auto it = cameraControlers.find(name);
	if(it == cameraControlers.end()) return;
	currentCameraControler = it->second;
	currentCameraControler->resize(Window::getSize());
	currentIsEntity = false;
}

void CameraSystem::setCurrentCamera(Entity e) {

}

void CameraSystem::update() {
	if(currentIsEntity) {

	} else {
		currentCameraControler->update();
	}
}

CameraData const *CameraSystem::getCameraData() {
	//if(currentIsEntity) {

	//} else {
		return currentCameraControler->getData();
	//}
}
