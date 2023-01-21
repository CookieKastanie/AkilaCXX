#pragma once

#include "akila/core/ecs/ecs.hpp"
#include "akila/engine/components/transform_component.hpp"

namespace akila {
	class SceneSystem: public System {
	public:
		SceneSystem();

		void updateTick();
		void updateFrame();

		void addChild(Entity parent, Entity child);
		void removeChild(Entity parent, Entity child);

	protected:
		void onAdd(Entity entity) override;
		void onRemove(Entity entity) override;

	private:
		std::vector<Entity> roots;
		void recursiveTraverTreeCalcMatrixMix(Entity e, Mat4 const &origin);
	};
}
