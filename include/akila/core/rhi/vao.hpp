#pragma once

#include "akila/core/rhi/buffer_object.hpp"
#include <functional>

namespace akila {
	class VAO {
	public:
		enum class Mode: GLuint {
			TRIANGLES = GL_TRIANGLES,
			TRIANGLE_FAN = GL_TRIANGLE_FAN,

			LINES = GL_LINES,
			LINE_STRIP = GL_LINE_STRIP,

			POINTS = GL_POINTS,
		};

		VAO();
		~VAO();

		VAO(VAO &&other) noexcept;
		VAO &operator=(VAO &&other) noexcept;

		VAO(VAO const &other) = delete;
		VAO &operator=(VAO const &other) = delete;

		void setDrawMode(Mode mode);
		void bind() const;
		void registerVBO(VBO const *vbo, unsigned int divisor = 0);
		void registerIBO(IBO const *ibo);
		void unbind() const;

		void draw() const;
		void draw(int length) const;

		void drawInstanced(int instanceCount) const;
		void drawInstanced(int instanceCount, int length) const;

	private:
		GLuint id;
		GLuint drawMode;
		int length;
		bool useIndices;
		GLenum indicesType;

		static void drawArray(VAO const &vao, int length);
		static void drawElements(VAO const &vao, int length);
		std::function<void(VAO const &vao, int length)> drawFunc;

		static void drawInstancedArray(VAO const &vao, int instanceCount, int length);
		static void drawInstancedElements(VAO const &vao, int instanceCount, int length);
		std::function<void(VAO const &vao, int instanceCount, int length)> drawInstancedFunc;
	};
}
