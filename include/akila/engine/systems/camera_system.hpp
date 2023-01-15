#pragma once

#include "akila/core/ecs/ecs.hpp"
#include "akila/engine/graphics/camera.hpp"
#include "akila/core/signals/signals.hpp"

namespace akila {
	class CameraSystem: public System {
	public:
		CameraSystem(Signature signature = ECS::createSignature<>());
		virtual ~CameraSystem() = default;

		virtual void update() = 0;

		virtual CameraData const *getCameraData() = 0;
		virtual Vec3 const &getDirection() = 0;

	protected:
		Listener resizeListener;

		virtual void onResize(IVec2 const &size) = 0;
	};
}
