#pragma once

#include <akila/akila.hpp>
#include <akila/default/components.hpp>
#include "mesh_component.hpp"

class RenderSystem: public akila::System {
public:
	static unsigned int const CAMERA_LOCATION = 0;

	akila::Ref<akila::Shader> skyboxShader;
	akila::Ref<akila::TextureCubmap> skyboxTexture;
	akila::Ref<akila::TextureCubmap> irradianceTexture;
	akila::Ref<akila::TextureCubmap> prefilterTexture;
	akila::Ref<akila::Texture2D> brdtLUTexture;
	akila::Ref<akila::StaticMesh> invertedCube;

	RenderSystem();

	void colorPass(akila::CameraComponent *cam);

private:
	akila::UBO cameraBuffer;


	//akila::FrameBuffer framebuffer;
};
