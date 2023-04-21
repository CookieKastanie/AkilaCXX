#pragma once

#include <glad/glad.h>
#include "akila/core/math/math.hpp"

namespace akila {
	class Renderer {
	public:
		static void bindDefaultFrameBuffer();

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

		enum class BlendEquation: GLenum {
			ADD = GL_FUNC_ADD,
			SUBTRACT = GL_FUNC_SUBTRACT,
			REVERSE_SUBTRACT = GL_FUNC_REVERSE_SUBTRACT,
			MIN = GL_MIN,
			MAX = GL_MAX
		};

		static void blendEquation(BlendEquation eq);

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
		static void setClearColor(Vec3 const &color);
		static void setClearColor(Vec4 const &color);
		static void clear();
		static void clearColor();
		static void clearDepth();
		static void clearStencil();

		enum class Barrier: GLbitfield {
			VERTEX_ATTRIB_ARRAY = GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT,
			ELEMENT_ARRAY = GL_ELEMENT_ARRAY_BARRIER_BIT,
			UNIFORM_BARRIER = GL_UNIFORM_BARRIER_BIT,
			TEXTURE_FETCH = GL_TEXTURE_FETCH_BARRIER_BIT,
			SHADER_IMAGE_ACCESS = GL_SHADER_IMAGE_ACCESS_BARRIER_BIT,
			COMMAND = GL_COMMAND_BARRIER_BIT,
			PIXEL_BUFFER = GL_PIXEL_BUFFER_BARRIER_BIT,
			TEXTURE_UPDATE = GL_TEXTURE_UPDATE_BARRIER_BIT,
			BUFFER_UPDATE = GL_BUFFER_UPDATE_BARRIER_BIT,
			FRAMEBUFFER = GL_FRAMEBUFFER_BARRIER_BIT,
			TRANSFORM_FEEDBACK = GL_TRANSFORM_FEEDBACK_BARRIER_BIT,
			ATOMIC_COUNTER = GL_ATOMIC_COUNTER_BARRIER_BIT,
			SHADER_STORAGE = GL_SHADER_STORAGE_BARRIER_BIT,
			//QUERY_BUFFER = GL_QUERY_BUFFER_BARRIER_BIT,
			ALL = GL_ALL_BARRIER_BITS,
		};

		static void memoryBarrier(Barrier barrier);

	private:
		friend class Core;

		static void init();
	};
}
