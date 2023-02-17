#include "akila/engine/systems/render_system.hpp"

using namespace akila;

RenderSystem::RenderSystem(): System{ECS::createSignature<TransformComponent, MeshComponent>()} {

}


DummyRenderSystem::DummyRenderSystem(): RenderSystem{} {

}

void DummyRenderSystem::updateFrame(CameraData const *) {

}
