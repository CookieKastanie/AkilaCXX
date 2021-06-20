#pragma once

#include "Akila/graphics/gl/Buffer.hpp"

namespace Akila {
	class VAO {
		private:
			GLuint id;
			GLuint drawMode;
			int length;

		public:
			static const unsigned int TRIANGLES = GL_TRIANGLES;
			static const unsigned int LINES = GL_LINES;

			VAO();
			~VAO();

			void setDrawMode(unsigned int mode);
			void bind() const;
			void registerVBO(const VBO *vbo);
			void unbind() const;

			void draw() const;
	};
}
