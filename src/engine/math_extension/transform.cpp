#include "akila/engine/math_extension/transform.hpp"

using namespace akila;

Transform::Transform():
	position{0.f},
	rotation{1.f, 0.f, 0.f, 0.f},
	scale{1.f},
	matrix{1.f} {
	
}

void Transform::translate(Vec3 const &vec) {
	position += vec;
}

void Transform::rotateX(float a) {
	rotation *= angleAxis(a, Vec3{1, 0, 0});
}

void Transform::rotateY(float a) {
	rotation *= angleAxis(a, Vec3{0, 1, 0});
}

void Transform::rotateZ(float a) {
	rotation *=  angleAxis(a, Vec3{0, 0, 1});
}

void Transform::rotate(Quat const &r) {
	rotation *= r;
}

void Transform::setPosition(Vec3 const &vec) {
	position = vec;
}

void Transform::setRotationZYX(Vec3 const &r) {
	rotation = {1, 0, 0, 0};
	rotateZ(r.z);
	rotateY(r.y);
	rotateX(r.x);
}

void Transform::setScale(float s) {
	scale = {s, s, s};
}

void Transform::setScale(Vec3 const &s) {
	scale = s;
}

Vec3 const &Transform::getPosition() {
	return position;
}

Mat4 const &Transform::calcMatrix() {
	matrix = toMat4(rotation);

	matrix[3].x = position.x;
	matrix[3].y = position.y;
	matrix[3].z = position.z; // affectation direct,
	// car une rotation n'a pas de translate de base

	matrix = akila::scale(matrix, scale);

	return matrix;
}

Mat4 const &Transform::calcMatrixFromOrigin(Mat4 const &o) {
	matrix = o * calcMatrix();
	return matrix;
}
