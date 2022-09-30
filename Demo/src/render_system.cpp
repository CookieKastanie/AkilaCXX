#include "render_system.hpp"
#include <akila/default/components.hpp>

using namespace akila;

RenderSystem::RenderSystem():
	System{ECS::createSignature<MeshComponent, TransformComponent>()},
	cameraBuffer{CAMERA_LOCATION, sizeof(CameraComponent)} {
	
}

void RenderSystem::colorPass() {
	Renderer::useDefaultFrameBuffer();
	Renderer::clear();

	for(Entity entity : entities) {
		
	}
}
