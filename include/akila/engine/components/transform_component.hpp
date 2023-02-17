#pragma once

#include "akila/engine/math_extension/transform.hpp"
#include "akila/core/time/time.hpp"
#include "akila/core/ecs/ecs.hpp"
#include "akila/engine/containers/small_vector.hpp"

namespace akila {
	class TransformComponent {
	public:
		TransformComponent();

		Vec3 &position();
		Quat &rotation();
		Vec3 &scale();

		void rotateX(float a);
		void rotateY(float a);
		void rotateZ(float a);

		Vec3 const &getPosition() const;
		Quat const &getRotation() const;
		Vec3 const &getScale() const;

		bool hasParent() const;
		Entity getParent() const;
		std::size_t getChildCount() const;
		Entity getChild(std::size_t index) const;

		Mat4 const &getWorldMatrix();

	private:
		friend class SceneSystem;

		Transform local;

		Vec3 prevLocalPosition;
		Quat prevLocalRotation;
		Vec3 prevLocalScale;

		Mat4 worldMatrix;

		Entity parent;
		SmallVector<Entity, 8> children;

		void savePrevious();
		Mat4 const &calcMatrixMix(float t = Time::mix);
		Mat4 const &calcMatrixMixFrom(Mat4 const & origin, float t = Time::mix);
	};
}
