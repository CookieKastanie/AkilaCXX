#include "akila/engine/systems/fast3d_render_system.hpp"
#include "akila/core/rhi/renderer.hpp"

using namespace akila;

Fast3dRenderSystem::Fast3dRenderSystem():
	RenderSystem{},
	cameraBuffer{CAMERA_LOCATION, sizeof(CameraData)} {

}

void Fast3dRenderSystem::colorPass(CameraData const *cameraData) {
	Renderer::bindDefaultFrameBuffer();
	Renderer::clear();

	cameraBuffer.setData(cameraData);

	for(Entity entity : entities) {
		auto &transform = entity.getComponent<TransformComponent>();
		auto &model = entity.getComponent<MeshComponent>();

		model.material.getBaseMaterial()->sendReserved("u_modelMatrix", transform.calcMatrixMix());
		model.material.send();
		model.mesh->draw();
	}
}
