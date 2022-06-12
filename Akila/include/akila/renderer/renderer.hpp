#pragma once

#include "glad/glad.h"

namespace akila {
	class Renderer {
	public:
		static void useDefaultFrameBuffer();

		enum class Capability: GLenum {
			BLEND = GL_BLEND,
			DEPTH_TEST = GL_DEPTH_TEST,
			TEXTURE_CUBE_MAP_SEAMLESS = GL_TEXTURE_CUBE_MAP_SEAMLESS,
			CULL_FACE = GL_CULL_FACE,
			SCISSOR_TEST = GL_SCISSOR_TEST
		};

		static void enable(Capability cap);
		static void disable(Capability cap);

		static void scissor(int x, int y, int width, int height);

		enum class BlendFactor: GLenum {
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

		static void blendFunc(BlendFactor sfactor, BlendFactor dfactor);


		enum class DepthFunc: GLenum {
			NEVER = GL_NEVER,
			LESS = GL_LESS,
			EQUAL = GL_EQUAL,
			LEQUAL = GL_LEQUAL,
			GREATER = GL_GREATER,
			NOTEQUAL = GL_NOTEQUAL,
			GEQUAL = GL_GEQUAL,
			ALWAYS = GL_ALWAYS
		};

		static void depthFunc(DepthFunc func);

		enum class Face: GLenum {
			FRONT = GL_FRONT,
			BACK = GL_BACK,
			FRONT_AND_BACK = GL_FRONT_AND_BACK
		};

		static void cullFace(Face face);

		static void setClearColor(float r, float g, float b, float a = 1.f);
		static void clear();
		static void clearColor();
		static void clearDepth();

	private:
		friend class Core;

		static void init();
	};
}
