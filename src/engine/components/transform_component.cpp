#include "akila/engine/components/transform_component.hpp"

using namespace akila;

TransformComponent::TransformComponent():
	local{},
	prevLocalPosition{0.f},
	prevLocalRotation{1.f, 0.f, 0.f, 0.f},
	prevLocalScale{1.f},
	worldMatrix{1.f},
	parent{},
	children{} {

}

Vec3 &TransformComponent::position() {
	return local.position;
}

Quat &TransformComponent::rotation() {
	return local.rotation;
}

Vec3 &TransformComponent::scale() {
	return local.scale;
}

void TransformComponent::rotateX(float a) {
	local.rotateX(a);
}

void TransformComponent::rotateY(float a) {
	local.rotateY(a);
}

void TransformComponent::rotateZ(float a) {
	local.rotateZ(a);
}

Vec3 const &TransformComponent::getPosition() const {
	return local.position;
}

Quat const &TransformComponent::getRotation() const {
	return local.rotation;
}

Vec3 const &TransformComponent::getScale() const {
	return local.scale;
}

bool TransformComponent::hasParent() const {
	return parent.isValid();
}

Entity TransformComponent::getParent() const {
	return parent;
}

std::size_t TransformComponent::getChildCount() const {
	return children.size();
}

Entity TransformComponent::getChild(std::size_t index) const {
	return children[index];
}

void TransformComponent::savePrevious() {
	prevLocalPosition = local.position;
	prevLocalRotation = local.rotation;
	prevLocalScale = local.scale;
}

Mat4 const &TransformComponent::calcMatrixMix(float t) {
	Vec3 lerpedPos = mix(prevLocalPosition, local.position, t);
	Quat lerpedRot = slerp(prevLocalRotation, local.rotation, t);
	Vec3 lerpedSca = mix(prevLocalScale, local.scale, t);

	worldMatrix = toMat4(lerpedRot);

	worldMatrix[3].x = lerpedPos.x;
	worldMatrix[3].y = lerpedPos.y;
	worldMatrix[3].z = lerpedPos.z;

	worldMatrix[0] *= lerpedSca[0];
	worldMatrix[1] *= lerpedSca[1];
	worldMatrix[2] *= lerpedSca[2];

	return worldMatrix;
}

Mat4 const &TransformComponent::calcMatrixMixFrom(Mat4 const &origin, float t) {
	worldMatrix = origin * calcMatrixMix(t);
	return worldMatrix;
}

Mat4 const &TransformComponent::getWorldMatrix() {
	return worldMatrix;
}
