#include "akila/default/components/transform_component.hpp"

using namespace akila;

TransformComponent::TransformComponent():
	prevPosition{0, 0, 0}, prevRotation{1, 0, 0, 0}, prevScale{1, 1, 1},
	position{0, 0, 0}, rotation{1, 0, 0, 0}, scale{1, 1, 1},
	matrix{1} {

}

void TransformComponent::translate(Vec3 const &vec) {
	position += vec;
}

void TransformComponent::rotateX(float a) {
	rotation *= angleAxis(a, Vec3{1, 0, 0});
}

void TransformComponent::rotateY(float a) {
	rotation *= angleAxis(a, Vec3{0, 1, 0});
}

void TransformComponent::rotateZ(float a) {
	rotation *=  angleAxis(a, Vec3{0, 0, 1});
}

void TransformComponent::setRotationZYX(Vec3 const &r) {
	rotation = {1, 0, 0, 0};
	rotateZ(r.z);
	rotateY(r.y);
	rotateX(r.x);
}

void TransformComponent::setScale(float s) {
	scale = {s, s, s};
}

Mat4 const &TransformComponent::calcMatrix() {
	matrix = toMat4(rotation);

	matrix[3].x = position.x;
	matrix[3].y = position.y;
	matrix[3].z = position.z; // affectation direct,
	// car une rotation n'a pas de translate de base

	matrix = akila::scale(matrix, scale);

	return matrix;
}

Mat4 const &TransformComponent::calcMatrixFromOrigin(Mat4 const &o) {
	matrix = o * calcMatrix();
	return matrix;
}

void TransformComponent::savePrevious() {
	prevPosition = position;
	prevRotation = rotation;
	prevScale = scale;
}

Mat4 const &TransformComponent::calcMatrixMix(float t) {
	Vec3 lerpedPos = mix(prevPosition, position, t);
	Quat lerpedRot = slerp(prevRotation, rotation, t);
	Vec3 lerpedSca = mix(prevScale, scale, t);

	matrix = toMat4(lerpedRot);

	matrix[3].x = lerpedPos.x;
	matrix[3].y = lerpedPos.y;
	matrix[3].z = lerpedPos.z;

	matrix = akila::scale(matrix, lerpedSca);

	return matrix;
}
