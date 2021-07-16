#include "Akila/graphics/Camera.hpp"

using namespace Akila;

Camera::Camera(): uniforms{
	glm::mat4{1.},
	glm::mat4{1.},
	glm::mat4{1.},
	glm::vec3{},
	1
} {}

Camera::~Camera() {}

void Camera::onResize(int width, int height) {
	uniforms.ratio = (float)width / (float)height;
}

void Camera::update() {}

void Camera::setPosition(const glm::vec3 &pos) {
	uniforms.position = pos;
}

glm::vec3 &Camera::getPosition() {
	return uniforms.position;
}

Camera::Uniforms &Camera::getUniforms() {
	return uniforms;
}

////////////////////////////////////////////////////////////

void PerspectiveCamera::onResize(int width, int height) {
	uniforms.ratio = (float)width / (float)height;
	uniforms.projection = glm::perspective(70.f, uniforms.ratio, 0.01f, 10.f);
}

void PerspectiveCamera::update() {
	uniforms.view = glm::lookAt(uniforms.position, {0.f, 0.f, 0.f}, {0.f, 1.f, 0.f});
	uniforms.pv = uniforms.projection * uniforms.view;
}
