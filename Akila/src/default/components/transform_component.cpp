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
	rotation *= angleAxis(a, Vec3{-1, 0, 0});
}

void TransformComponent::rotateY(float a) {
	rotation *= angleAxis(a, Vec3{0, -1, 0});
}

void TransformComponent::rotateZ(float a) {
	rotation *=  angleAxis(a, Vec3{0, 0, -1});
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
	Mat4 &&rotate = toMat4(rotation);
	Mat4 &&translate = akila::translate(Mat4(1.), position);
	Mat4 &&scaled = akila::scale(Mat4(1.), scale);
	matrix = translate * rotate * scaled;

	return matrix;
}

Mat4 const &TransformComponent::calcMatrixFromOrigin(Mat4 const &o) {
	Mat4 &&rotate = toMat4(rotation);
	Mat4 &&translate = akila::translate(Mat4(1.f), position);
	Mat4 &&scaled = akila::scale(Mat4(1.f), scale);
	matrix = o * translate * rotate * scaled;

	return matrix;
}

void TransformComponent::savePrevious() {
	prevPosition = position;
	prevRotation = rotation;
	prevScale = scale;
}

Mat4 const &TransformComponent::calcMatrixMix(float t) {
	Vec3 &&lerpedPos = mix(prevPosition, position, t);
	Quat &&lerpedRot = slerp(prevRotation, rotation, t);
	Vec3 &&lerpedSca = mix(prevScale, scale, t);

	Mat4 &&rotate = toMat4(lerpedRot);
	Mat4 &&translate = akila::translate(Mat4(1.f), lerpedPos);
	Mat4 &&scaled = akila::scale(Mat4(1.f), lerpedSca);
	matrix = translate * rotate * scaled;

	return matrix;
}
