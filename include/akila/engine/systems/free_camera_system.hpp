#pragma once

#include "akila/engine/systems/camera_system.hpp"

namespace akila {
	class FreeCameraSystem: public CameraSystem {
	public:
		FreeCameraSystem();

		void update() override;
		CameraData const *getCameraData() override;

	protected:
		void onResize(IVec2 const &size) override;

	private:
		struct FreeCamera: Camera {
			Vec3 center{0.f};
			Vec3 angles{0.f, HALF_PI, 0.f};
			float distance{10.f};

			float maxDistance{100.f};
		} camera;
	};
}
