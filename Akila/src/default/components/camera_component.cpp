#include "akila/default/components/camera_component.hpp"

using namespace akila;

CameraComponent::CameraComponent():
	projection{1.f},
	view{1.f},
	pv{1.f},
	position{0.f},
	ratio{1.f}
{}

//

PerspectiveCameraComponent::PerspectiveCameraComponent():
	CameraComponent{},
	fov{45},
	near{0.01f},
	far{1000.f}
{}

void PerspectiveCameraComponent::resize(IVec2 const &viewportSize) {
	float x = static_cast<float>(viewportSize.x);
	float y = static_cast<float>(viewportSize.y);

	ratio = x / y;
	recalcProjection();
}

void PerspectiveCameraComponent::recalcProjection() {
	projection = perspective(fov, ratio, near, far);
}

//

OrbitCameraComponent::OrbitCameraComponent():
	PerspectiveCameraComponent{},
	up{0.f},
	center{0.f},
	angles{0.f, HALF_PI, 0.f},
	movement{0.f},
	distance{3.f}
{}
