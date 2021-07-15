#pragma once

#include <glad/glad.h>
#include <vector>
#include <string>
#include <iostream>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"

#include <functional>

namespace Akila {
	class Shader {
		private:
			static void(*funifFuncs[5])(GLint, GLsizei, GLfloat*);
			static void(*iunifFuncs[5])(GLint, GLsizei, GLint*);

			GLuint id;

			bool checkErrors(GLuint shader, std::string type);

		public:
			static void funcInit();

			Shader(const std::string &vertexCode, const std::string &fragmentCode, const std::string &geometryCode = "");
			~Shader();

			void bind() const;

			void setUBOIndex(const std::string &name, unsigned int index);

			unsigned int getUniformId(const std::string &name);

			void send(const unsigned int &uid, const int &value) const;
			void send(const unsigned int &uid, const float &value) const;
			void send(const unsigned int &uid, const glm::vec2 &value) const;
			void send(const unsigned int &uid, const glm::vec3 &value) const;
			void send(const unsigned int &uid, const std::vector<glm::vec3> &values) const;
			void send(const unsigned int &uid, const glm::mat4 &mat) const;
			void send(const unsigned int &uid, const bool &value) const;

			void sendRawFloat(const unsigned int &uid, const void *values, const int &funcId) const;
			void sendRawInt(const unsigned int &uid, const void *values, const int &funcId) const;
	};
}
