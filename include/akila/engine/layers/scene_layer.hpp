#pragma once

#include "akila/core/layer/layer.hpp"
#include "akila/engine/systems/render_system.hpp"

namespace akila {
	class SceneLayer: public Layer {
	public:
		SceneLayer();

		void tick() final;
		void frame() final;
		void gui() final;

	private:
		RenderSystem *renderSystem;
	};
}
