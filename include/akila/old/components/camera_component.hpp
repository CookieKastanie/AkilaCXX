#pragma once

#include "akila/math/math.hpp"

namespace akila {
	struct CameraComponent {
		CameraComponent();

		Mat4 projection;
		Mat4 view;
		Mat4 pv;
		Vec3 position;
		float ratio;
	};

	struct PerspectiveCameraComponent: public CameraComponent {
		PerspectiveCameraComponent();

		void resize(IVec2 const &viewportSize);
		void recalcProjection();

		float fov;
		float near;
		float far;
	};

	struct OrbitCameraComponent: public PerspectiveCameraComponent {
		OrbitCameraComponent();

		Vec3 up;
		Vec3 center;
		Vec3 angles;
		Vec3 movement;
		float distance;
	};
}
