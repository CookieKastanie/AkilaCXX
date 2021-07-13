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

		public:
			Renderer(std::shared_ptr<Display> &Display);

			void prepare();
			void useDefaultFrameBuffer();

			void setSharedCamera(const std::shared_ptr<Camera> &cam);
			std::shared_ptr<Camera> &getSharedCamera();

			void affectUBOToShader(Shader *shader);


			enum Capability: GLenum {
				BLEND = GL_BLEND,
				DEPTH_TEST = GL_DEPTH_TEST,
				TEXTURE_CUBE_MAP_SEAMLESS = GL_TEXTURE_CUBE_MAP_SEAMLESS,
				CULL_FACE = GL_CULL_FACE
			};

			void enable(Capability cap);
			void disable(Capability cap);


			enum BlendFactor: GLenum {
				ZERO = GL_ZERO,
				ONE = GL_ONE,
				SRC_COLOR = GL_SRC_COLOR,
				ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
				DST_COLOR = GL_DST_COLOR,
				ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
				SRC_ALPHA = GL_SRC_ALPHA,
				ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
				DST_ALPHA = GL_DST_ALPHA,
				ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
				CONSTANT_COLOR = GL_CONSTANT_COLOR,
				ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR,
				CONSTANT_ALPHA = GL_CONSTANT_ALPHA,
				ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA,
				SRC_ALPHA_SATURATE = GL_SRC_ALPHA_SATURATE
			};

			void blendFunc(BlendFactor sfactor, BlendFactor dfactor);

			void clearDepth();
	};
}
