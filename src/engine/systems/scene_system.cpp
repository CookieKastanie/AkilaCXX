#include "akila/engine/systems/scene_system.hpp"

using namespace akila;

SceneSystem::SceneSystem(): System{ECS::createSignature<TransformComponent>()} {

}

void SceneSystem::onAdd(Entity entity) {

}

void SceneSystem::onRemove(Entity entity) {

}

void SceneSystem::update() {

}
