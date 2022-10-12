#include "render_system.hpp"
#include <akila/default/components.hpp>

using namespace akila;

RenderSystem::RenderSystem():
	System{ECS::createSignature<MeshComponent, TransformComponent>()},
	cameraBuffer{CAMERA_LOCATION, sizeof(CameraComponent)} {
	
}

void RenderSystem::colorPass(CameraComponent *cam) {
	Renderer::useDefaultFrameBuffer();
	Renderer::clear();

	cameraBuffer.setData(cam);

	for(Entity entity : entities) {
		auto &transform = entity.getComponent<TransformComponent>();
		auto &mesh = entity.getComponent<MeshComponent>();

		mesh.material.getShader()->send("u_modelMatrix", transform.calcMatrixMix());
		mesh.material.send();
		mesh.mesh->draw();
	}
}
