#pragma once

#include "akila/core/layer/layer.hpp"
#include "akila/engine/systems/render_system.hpp"
#include "akila/engine/systems/audio_system.hpp"
#include "akila/engine/systems/scene_system.hpp"
#include "akila/engine/systems/camera_system.hpp"

namespace akila {
	class SceneLayer: public Layer {
	public:
		SceneLayer() = default;
		virtual ~SceneLayer() = default;

		virtual void onMount() override;

		virtual void tick() override;
		virtual void frame() override;

	protected:
		SceneSystem *sceneSystem;
		RenderSystem *renderSystem;
		AudioSystem *audioSystem;
		CameraSystem *cameraSystem;
	};
}
