#pragma once

#include "akila/ecs/ecs.hpp"

namespace akila {
	class OrbitCameraSystem: public System {
	public:
		OrbitCameraSystem();
		void update();
	};
}
