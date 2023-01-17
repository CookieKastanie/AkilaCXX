#pragma once

#include "akila/engine/math_extension/transform.hpp"
#include "akila/core/time/time.hpp"
#include "akila/core/ecs/ecs.hpp"
#include "akila/engine/containers/small_vector.hpp"

namespace akila {
	class TransformComponent: public Transform {
	public:
		TransformComponent();
		void savePrevious();
		Mat4 const &calcMatrixMix(float t = Time::mix);

		bool isRoot();
		Entity getParent();
		std::size_t getChildrenCount();
		Entity getChild(std::size_t index);

	private:
		Vec3 prevPosition;
		Quat prevRotation;
		Vec3 prevScale;

		friend class TransformComponentParenting;
		Entity parent;
		SmallVector<Entity, 8> children;
	};

	class TransformComponentParenting {
	public:
		static void addChild(Entity perent, Entity child);
		static void removeChild(Entity perent, Entity child);
	};
}
