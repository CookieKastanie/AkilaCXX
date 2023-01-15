#include "akila/engine/systems/camera_system.hpp"
#include "akila/core/window/window_events.hpp"

using namespace akila;

CameraSystem::CameraSystem(Signature signature):
	System{signature} {

	resizeListener = Signals::listen<WindowResizeSignal>([this](WindowResizeSignal const &s) {
		onResize(s.size);
	});
}
