#pragma once

#include <glad/glad.h>
#include <vector>
#include <string>
#include <iostream>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"

namespace Akila {
	class Shader {
		private:
			GLuint id;

			bool checkErrors(GLuint shader, std::string type);

		public:
			Shader(const std::string &vertexCode, const std::string &fragmentCode, const std::string &geometryCode = "");
			~Shader();

			void bind() const;

			void send(unsigned int uid, int value) const;
			void send(unsigned int uid, float value) const;
			void send(unsigned int uid, const glm::vec2 &value) const;
			void send(unsigned int uid, const glm::vec3 &value) const;
			void send(unsigned int uid, const std::vector<glm::vec3> &values) const;
			void send(unsigned int uid, const glm::mat4 &mat) const;
			void send(unsigned int uid, const bool value) const;
	};
}
