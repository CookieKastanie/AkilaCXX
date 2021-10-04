#pragma once

#include "Akila/graphics/gl/Buffer.hpp"
#include <functional>

namespace Akila {
	class VAO {
		private:
			GLuint id;
			GLuint drawMode;
			int length;
			bool useIndices;
			GLenum indicesType;

			inline static void drawArray(const VAO &vao);
			inline static void drawElements(const VAO &vao);
			std::function<void(const VAO &vao)> drawFunc;

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
			void registerVBO(const VBO *vbo);
			void registerIBO(const IBO *ibo);
			void unbind() const;

			void draw() const;
	};
}
