#include "akila/engine/components/transform_component.hpp"

using namespace akila;

TransformComponent::TransformComponent():
	Transform{},
	prevPosition{0.f},
	prevRotation{1.f, 0.f, 0.f, 0.f},
	prevScale{1.f} {

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
