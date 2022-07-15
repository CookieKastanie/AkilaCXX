#pragma once

#include "akila/renderer/buffer_object.hpp"
#include <functional>

namespace akila {
	class VAO {
	private:
		GLuint id;
		GLuint drawMode;
		int length;
		bool useIndices;
		GLenum indicesType;

		static void drawArray(VAO const &vao);
		static void drawElements(VAO const &vao);
		std::function<void(VAO const &vao)> drawFunc;

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

		void setDrawMode(Mode mode);
		void bind() const;
		void registerVBO(VBO const *vbo);
		void registerIBO(IBO const *ibo);
		void unbind() const;

		void draw() const;
	};
}