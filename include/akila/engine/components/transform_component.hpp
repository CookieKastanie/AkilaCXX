#pragma once

#include "akila/engine/math_extension/transform.hpp"
#include "akila/core/time/time.hpp"

namespace akila {
	class TransformComponent: public Transform {
	public:
		void savePrevious();
		Mat4 const &calcMatrixMix(float t = Time::mix);

	private:
		Vec3 prevPosition;
		Quat prevRotation;
		Vec3 prevScale;
	};
}
