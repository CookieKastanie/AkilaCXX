#pragma once

#include "akila/engine/graphics/camera.hpp"

namespace akila {
	class CameraControler: public Camera {
	public:
		CameraControler();
		virtual ~CameraControler() = default;

		virtual void update() = 0;
	};

	class FreeCameraControler: public CameraControler {
	public:
		FreeCameraControler();

		void update() override;

	private:
		Vec3 center;
		Vec3 angles;
		float distance;

		float maxDistance;
	};
}
