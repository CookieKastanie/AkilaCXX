#include "Akila/graphics/Camera.hpp"

using namespace Akila;

Camera::Camera(): uniforms{
	glm::mat4{},
	glm::mat4{},
	glm::mat4{},
	glm::vec3{}
} {}

Camera::~Camera() {}

void Camera::onResize() {}

void Camera::update() {}

void Camera::setPosition(glm::vec3 &pos) {
	uniforms.position = pos;
}

glm::vec3 &Camera::getPosition() {
	return uniforms.position;
}

Camera::Uniforms &Camera::getUniforms() {
	return uniforms;
}
