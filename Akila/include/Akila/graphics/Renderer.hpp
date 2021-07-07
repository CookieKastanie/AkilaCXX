#pragma once

#include "glad/glad.h"
#include <memory>
#include "Akila/core/Display.hpp"
#include "Akila/graphics/Camera.hpp"
#include "Akila/graphics/gl/Buffer.hpp"
#include "Akila/graphics/Material.hpp"
#include <map>

namespace Akila {
	class Renderer {
		private:
			std::shared_ptr<Display> display;
			std::shared_ptr<Camera> camera;

			std::shared_ptr<UBO> cameraUBO;
			std::shared_ptr<UBO> timeUBO;

			std::map<std::string, std::shared_ptr<Material>> materials;

		public:
			Renderer(std::shared_ptr<Display> &Display);

			void prepare();
			void useDefaultFrameBuffer();

			void setSharedCamera(const std::shared_ptr<Camera> &cam);
			std::shared_ptr<Camera> &getSharedCamera();

			void loadMaterialFromFile(const std::string &fileName);
			std::shared_ptr<Material> &getMaterialbyName(const std::string &name);
	};
}
