#pragma once

#include "akila/core/ecs/ecs.hpp"
#include "akila/engine/components/transform_component.hpp"
#include "akila/engine/components/audio_component.hpp"
#include "akila/engine/graphics/camera.hpp"

namespace akila {
	class AudioSystem: public System {
	public:
		AudioSystem();

		void updateTick();
		void updateFrame(CameraData const *camera);

		void detach(Entity e);

	private:
		Vec3 lastCameraPosition;
	};
}
