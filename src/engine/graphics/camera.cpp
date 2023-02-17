#include "akila/engine/graphics/camera.hpp"

using namespace akila;

CameraData::CameraData():
	projection{1.f},
	view{1.f},
	pv{1.f},
	position{0.f},
	ratio{1.f} {

}

CameraData const *CameraData::getData() {
	return this;
}


///


Camera::Camera(ProjectionType type):
	CameraData{},
	type{type},
	near{0.1f},
	far{500.f},
	fov{toRadians(70.f)} {

}

void Camera::resize(IVec2 const &size) {
	float x = static_cast<float>(size.x);
	float y = static_cast<float>(size.y);

	ratio = x / y;
	refreshProjection();
}

void Camera::refreshProjection() {
	if(type == ProjectionType::PERSPRECTIVE) {
		projection = perspective(fov, ratio, near, far);
	} else {
		//projection = ortho();
	}
}
