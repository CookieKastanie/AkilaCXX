#pragma once

#include "akila/core/math/math.hpp"
#include "akila/core/time/time.hpp"

namespace akila {
	struct Transform {
		Transform();

		void translate(Vec3 const &vec);

		void rotateX(float a);
		void rotateY(float a);
		void rotateZ(float a);

		void setRotationZYX(Vec3 const &r);

		void setScale(float s);

		Mat4 const &calcMatrix();
		Mat4 const &calcMatrixFromOrigin(Mat4 const &o);

		void savePrevious();
		Mat4 const &calcMatrixMix(float t = Time::mix);

		Vec3 prevPosition;
		Quat prevRotation;
		Vec3 prevScale;

		Vec3 position;
		Quat rotation;
		Vec3 scale;

		Mat4 matrix;
	};
}
