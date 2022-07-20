#pragma once

#include "akila/math/math.hpp"
#include "akila/time/time.hpp"

namespace akila {
	class TransformComponent {
	public:
		TransformComponent();

		void setPosition(Vec3 const &vec);
		void translate(Vec3 const &vec);

		Vec3 const &getPosition();

		void rotateX(float a);
		void rotateY(float a);
		void rotateZ(float a);

		void setRotationZYX(Vec3 const &r);
		Quat const &getRotation();

		void setScale(Vec3 const &scale);
		void setScale(float s);

		Mat4 const &toMatrix();
		Mat4 const &toMatrixFromOrigin(Mat4 const &o);

		void savePrevious();
		Mat4 const &toMatrixMix(float t = Time::mix);

	private:
		Vec3 prevPosition;
		Quat prevRotation;
		Vec3 prevScale;

		Vec3 position;
		Quat rotation;
		Vec3 scale;

		Mat4 matrix;
	};
}
