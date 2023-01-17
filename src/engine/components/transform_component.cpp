#include "akila/engine/components/transform_component.hpp"

using namespace akila;

TransformComponent::TransformComponent():
	Transform{},
	prevPosition{0.f},
	prevRotation{1.f, 0.f, 0.f, 0.f},
	prevScale{1.f},
	parent{},
	children{} {

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

bool TransformComponent::isRoot() {
	return parent.isValid() == false;
}

Entity TransformComponent::getParent() {
	return parent;
}

std::size_t TransformComponent::getChildrenCount() {
	return children.size();
}

Entity TransformComponent::getChild(std::size_t index) {
	return children[index];
}

void TransformComponentParenting::addChild(Entity perent, Entity child) {
	if(perent.hasComponent<TransformComponent>() == false) return;
	if(child.hasComponent<TransformComponent>() == false) return;

	auto &ta = perent.getComponent<TransformComponent>();
	auto &tb = child.getComponent<TransformComponent>();

	ta.children.push_back(child);
	tb.parent = perent;
}

void TransformComponentParenting::removeChild(Entity perent, Entity child) {
	if(perent.hasComponent<TransformComponent>() == false) return;
	if(child.hasComponent<TransformComponent>() == false) return;

	auto &ta = perent.getComponent<TransformComponent>();
	auto &tb = child.getComponent<TransformComponent>();

	for(auto it = ta.children.begin(); it != ta.children.end(); ++it) {
		if(*it == child) {
			ta.children.erase(it);
			tb.parent = Entity{};
			break;
		}
	}
}
