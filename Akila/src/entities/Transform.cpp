#include "Akila/entities/Transform.hpp"

using namespace Akila;

Transform::Transform(): position{0, 0, 0}, rotation{1, 0, 0, 0}, matrix{1} {}

void Transform::translate(float dx, float dy, float dz) {
	position += glm::vec3{dx, dy, dz};
}

void Transform::setPosition(float x, float y, float z) {
	position = {x, y, z};
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

glm::mat4x4 &Transform::toMatrix() {
	glm::mat4 &&rotate = glm::toMat4(rotation);
	glm::mat4 &&translate = glm::translate(glm::mat4(1.), position);
	matrix = translate * rotate;
	return matrix;
}

glm::mat4 &Transform::toMatrixFromOrigin(const glm::mat4 &o) {
	glm::mat4 &&rotate = glm::toMat4(rotation);
	glm::mat4 &&translate = glm::translate(glm::mat4(1.), position);
	matrix = o * translate * rotate;
	return matrix;
}

void Transform::savePrevious() {
	prevPosition = position;
	prevRotation = rotation;
}

glm::mat4 &Transform::toMatrixMix(const float t) {
	glm::vec3 &&lerpedPos = glm::mix(prevPosition, position, t);
	glm::quat &&lerpedRot = glm::slerp(prevRotation, rotation, t);
	glm::mat4 &&rotate = glm::toMat4(lerpedRot);
	glm::mat4 &&translate = glm::translate(glm::mat4(1.), lerpedPos);
	matrix = translate * rotate;
	return matrix;
}
