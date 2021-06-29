#pragma once

#include "glad/glad.h"
#include <memory>
#include "Akila/core/Display.hpp"
#include "Akila/graphics/Camera.hpp"
#include "Akila/graphics/gl/Buffer.hpp"

namespace Akila {
	class Renderer {
		private:
			std::shared_ptr<Display> display;
			std::shared_ptr<Camera> camera;

			std::shared_ptr<UBO> cameraUBO;

		public:
			Renderer(std::shared_ptr<Display> &Display);

			void prepare();
			void useDefaultFrameBuffer();
			std::shared_ptr<Camera> &getSharedCamera();
	};
}
