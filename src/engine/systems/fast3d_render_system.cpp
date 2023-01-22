#include "akila/engine/systems/fast3d_render_system.hpp"
#include "akila/core/rhi/renderer.hpp"

#include "akila/engine/graphics/material_factory.hpp"
#include "akila/engine/graphics/shaders/common/template_header.glsl"
#include "akila/engine/graphics/shaders/common/camera_struct.glsl"
#include "akila/engine/graphics/shaders/fast3d_template.glsl"

using namespace akila;

void Fast3dRenderSystem::injectShaderSources() {
	MaterialFactory::setSource("template_header", GLSL_TEMPLATE_HEADER);
	MaterialFactory::setSource("camera_struct", GLSL_CAMERA_STRUCT);
	MaterialFactory::setSource("fast3d_template", GLSL_FAST3D_TEMPLATE);

	MaterialFactory::define("u_camera_loc", CAMERA_LOCATION);

	MaterialFactory::reserveUniform("u_modelMatrix");
}

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

		model.material.getBaseMaterial()->sendReserved("u_modelMatrix", transform.getWorldMatrix());
		model.material.send();
		model.mesh->draw();
	}
}
