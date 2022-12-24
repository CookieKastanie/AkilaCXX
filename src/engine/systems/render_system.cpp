#include "akila/engine/systems/render_system.hpp"
#include "akila/engine/components/transform_component.hpp"
#include "akila/engine/components/mesh_component.hpp"

using namespace akila;

RenderSystem::RenderSystem(): System{ECS::createSignature<TransformComponent, MeshComponent>()} {

}
