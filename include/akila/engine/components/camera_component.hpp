#pragma once

#include "akila/engine/graphics/camera.hpp"

namespace akila {
	class CameraComponent: public Camera {
	public:
		CameraComponent();

		unsigned int getId();

	private:
		static unsigned int nextId;
		unsigned int id;
	};
}
