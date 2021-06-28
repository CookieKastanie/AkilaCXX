#pragma once

#include "glad/glad.h"
#include <memory>
#include "Akila/core/Display.hpp"
#include "Akila/graphics/Camera.hpp"

namespace Akila {
	class Renderer {
		private:
			std::shared_ptr<Display> display;
			//Camera *camera;

		public:
			Renderer(std::shared_ptr<Display> &Display);

			void useDefaultFrameBuffer();
	};
}
