#pragma once

#include "akila/engine/systems/render_system.hpp"
#include "akila/core/rhi/buffer_object.hpp"

namespace akila {
	class Fast3dRenderSystem: public RenderSystem {
	public:
		static unsigned int const CAMERA_LOCATION = 0;

		static void injectShaderSources();

		Fast3dRenderSystem();

		void updateFrame(CameraData const *cameraData) override;

	private:
		UBO cameraBuffer;
	};
}
