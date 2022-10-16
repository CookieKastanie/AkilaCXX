#include "render_system.hpp"
#include <akila/default/components.hpp>

using namespace akila;

RenderSystem::RenderSystem():
	System{ECS::createSignature<MeshComponent, TransformComponent>()},
	cameraBuffer{CAMERA_LOCATION, sizeof(CameraComponent)} {
	
	//framebuffer.setTexture(Resources::create<Texture2D>("color_target", TextureBuffer::Format::RGB), 0);
	//framebuffer.setDepthTexture(Resources::create<Texture2D>("depth_target", TextureBuffer::Format::DEPTH_COMPONENT));
	//framebuffer.resizeAll(Window::getSize());
	//framebuffer.prepare();
}

void RenderSystem::colorPass(CameraComponent *cam) {
	//framebuffer.bind();
	Renderer::bindDefaultFrameBuffer();
	Renderer::clear();

	cameraBuffer.setData(cam);

	for(Entity entity : entities) {
		auto &transform = entity.getComponent<TransformComponent>();
		auto &mesh = entity.getComponent<MeshComponent>();

		mesh.material.getShader()->send("u_modelMatrix", transform.calcMatrixMix());
		mesh.material.send();
		mesh.mesh->draw();
	}

	//framebuffer.blitToDefault(0, TextureBuffer::FilterMode::NEAREST);
	//framebuffer.blitToDefault(0);
}
