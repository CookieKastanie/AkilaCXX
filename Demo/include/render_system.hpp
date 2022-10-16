#pragma once

#include <akila/akila.hpp>
#include <akila/default/components.hpp>
#include "mesh_component.hpp"

class RenderSystem: public akila::System {
public:
	static unsigned int const CAMERA_LOCATION = 0;

	RenderSystem();

	void colorPass(akila::CameraComponent *cam);

private:
	akila::UBO cameraBuffer;

	//akila::FrameBuffer framebuffer;
};
