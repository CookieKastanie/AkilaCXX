#pragma once

#include "akila/core/ecs/ecs.hpp"
#include "akila/engine/graphics/camera.hpp"
#include "akila/engine/components/transform_component.hpp"
#include "akila/engine/components/mesh_component.hpp"

namespace akila {
	class RenderSystem: public System {
	public:
		RenderSystem();

		virtual void updateFrame(CameraData const *cameraData) = 0;
	};

	class DummyRenderSystem: public RenderSystem {
	public:
		DummyRenderSystem();

		void updateFrame(CameraData const *cameraData) override;
	};
}
