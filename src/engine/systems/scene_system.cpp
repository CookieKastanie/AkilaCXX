#include "akila/engine/systems/scene_system.hpp"

using namespace akila;

SceneSystem::SceneSystem(): System{ECS::createSignature<TransformComponent>()} {

}

void SceneSystem::onAdd(Entity entity) {
	auto &t = entity.getComponent<TransformComponent>();
	if(t.parent.isValid() == false) {
		roots.push_back(entity);
	}
}

void SceneSystem::onRemove(Entity entity) {
	auto &t = entity.getComponent<TransformComponent>();
	if(t.parent.isValid()) {
		removeChild(t.parent, entity);
	}

	roots.erase(std::remove(roots.begin(), roots.end(), entity), roots.end());

	//onRemove will be recursively called
	for(Entity child : t.children) {
		ECS::eraseEntity(child);
	}
}

void SceneSystem::addChild(Entity parent, Entity child) {
	if(parent.hasComponent<TransformComponent>() == false) return;
	if(child.hasComponent<TransformComponent>() == false) return;

	auto &ta = parent.getComponent<TransformComponent>();
	auto &tb = child.getComponent<TransformComponent>();

	//if already have parent
	if(tb.parent.isValid()) {
		removeChild(tb.parent, child);
	}

	ta.children.push_back(child);
	tb.parent = parent;

	// remove root
	roots.erase(std::remove(roots.begin(), roots.end(), child), roots.end());
}

void SceneSystem::removeChild(Entity parent, Entity child) {
	if(parent.hasComponent<TransformComponent>() == false) return;
	if(child.hasComponent<TransformComponent>() == false) return;

	auto &ta = parent.getComponent<TransformComponent>();
	auto &tb = child.getComponent<TransformComponent>();

	for(auto it = ta.children.begin(); it != ta.children.end(); ++it) {
		if(*it == child) {
			ta.children.erase(it);
			tb.parent = Entity{};
			break;
		}
	}

	// add root if not present
	if(std::find(roots.begin(), roots.end(), child) == roots.end()) {
		roots.push_back(child);
	}
}

void SceneSystem::recursiveTraverTreeCalcMatrixMix(Entity e, Mat4 const &origin) {
	auto &t = e.getComponent<TransformComponent>();
	Mat4 const &m = t.calcMatrixMixFrom(origin);

	for(Entity child : t.children) {
		recursiveTraverTreeCalcMatrixMix(child, m);
	}
}

void SceneSystem::updateTick() {
	for(Entity e : entities) {
		auto &t = e.getComponent<TransformComponent>();
		t.savePrevious();
	}

	ECS::flushEraseQueue();
}

void SceneSystem::updateFrame() {
	for(Entity e : roots) {
		auto &t = e.getComponent<TransformComponent>();
		Mat4 const &rootMatrix = t.calcMatrixMix();

		for(Entity child : t.children) {
			recursiveTraverTreeCalcMatrixMix(child, rootMatrix);
		}
	}
}
