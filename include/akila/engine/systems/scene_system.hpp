#pragma once

#include "akila/core/ecs/ecs.hpp"
#include "akila/engine/components/transform_component.hpp"

namespace akila {
	class SceneSystem: public System {
	public:
		SceneSystem();

		void update();

	protected:
		void onAdd(Entity entity) override;
		void onRemove(Entity entity) override;
	};
}
