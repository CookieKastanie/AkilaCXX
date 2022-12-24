#pragma once

#include "akila/core/math/math.hpp"

namespace akila {
	class Transform {
	public:
		Transform();

		void translate(Vec3 const &vec);

		void rotateX(float a);
		void rotateY(float a);
		void rotateZ(float a);

		void rotate(Quat const &r);

		void setRotationZYX(Vec3 const &r);

		void setScale(float s);
		void setScale(Vec3 const &s);

		Mat4 const &calcMatrix();
		Mat4 const &calcMatrixFromOrigin(Mat4 const &o);

	protected:
		Vec3 position;
		Quat rotation;
		Vec3 scale;

		Mat4 matrix;
	};
}
