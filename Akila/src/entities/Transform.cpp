#include "Akila/entities/Transform.hpp"

using namespace Akila;

Transform::Transform(): 
	prevPosition{0, 0, 0}, prevRotation{1, 0, 0, 0}, prevScale{1, 1, 1},
	position{0, 0, 0}, rotation{1, 0, 0, 0}, scale{1, 1, 1},
	matrix{1} {

}

void Transform::translate(float dx, float dy, float dz) {
	position += glm::vec3{dx, dy, dz};
}

void Transform::setPosition(float x, float y, float z) {
	position = {x, y, z};
}

void Transform::setPosition(glm::vec3 const &vec) {
	position = vec;
}

glm::vec3 &Transform::getPosition() {
	return position;
}

void Transform::rotateX(float a) {
	rotation *= glm::angleAxis(a, glm::vec3{-1, 0, 0});
}

void Transform::rotateY(float a) {
	rotation *= glm::angleAxis(a, glm::vec3{0, -1, 0});
}

void Transform::rotateZ(float a) {
	rotation *= glm::angleAxis(a, glm::vec3{0, 0, -1});
}

void Transform::setRotationZYX(float ax, float ay, float az) {
	rotation = {1, 0, 0, 0};
	rotateZ(az);
	rotateY(ay);
	rotateX(ax);
}

glm::quat &Transform::getRotation() {
	return rotation;
}

void Transform::setScale(float x, float y, float z) {
	scale = {x, y, z};
}

void Transform::setScale(float s) {
	scale = {s, s, s};
}

glm::mat4x4 &Transform::toMatrix() {
	glm::mat4 &&rotate = glm::toMat4(rotation);
	glm::mat4 &&translate = glm::translate(glm::mat4(1.), position);
	glm::mat4 &&scaled = glm::scale(glm::mat4(1.), scale);
	matrix = translate * rotate * scaled;
	return matrix;
}

glm::mat4 &Transform::toMatrixFromOrigin(const glm::mat4 &o) {
	glm::mat4 &&rotate = glm::toMat4(rotation);
	glm::mat4 &&translate = glm::translate(glm::mat4(1.), position);
	glm::mat4 &&scaled = glm::scale(glm::mat4(1.), scale);
	matrix = o * translate * rotate * scaled;
	return matrix;
}

void Transform::savePrevious() {
	prevPosition = position;
	prevRotation = rotation;
	prevScale = scale;
}

glm::mat4 &Transform::toMatrixMix(const float t) {
	glm::vec3 &&lerpedPos = glm::mix(prevPosition, position, t);
	glm::quat &&lerpedRot = glm::slerp(prevRotation, rotation, t);
	glm::vec3 &&lerpedSca = glm::mix(prevScale, scale, t);

	glm::mat4 &&rotate = glm::toMat4(lerpedRot);
	glm::mat4 &&translate = glm::translate(glm::mat4(1.), lerpedPos);
	glm::mat4 &&scaled = glm::scale(glm::mat4(1.), lerpedSca);
	matrix = translate * rotate * scaled;
	return matrix;
}
