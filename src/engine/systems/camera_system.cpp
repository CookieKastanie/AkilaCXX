#include "akila/engine/systems/camera_system.hpp"
#include "akila/core/window/window_events.hpp"

using namespace akila;

CameraSystem::CameraSystem(Signature signature):
	System{signature} {

	resizeListener = Signals::listen<WindowResizeSignal>([this](WindowResizeSignal const &s) {
		onResize(s.size);
	});
}


DummyCameraSystem::DummyCameraSystem(): CameraSystem{} {

}

void DummyCameraSystem::update() {

}

CameraData const *DummyCameraSystem::getCameraData() {
	return &cameraData;
}

Vec3 const &DummyCameraSystem::getDirection() {
	return {0.f, 0.f, -1.f};
}

void DummyCameraSystem::onResize(IVec2 const &) {

}
