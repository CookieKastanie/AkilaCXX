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
#include <unordered_map>

namespace Akila {
	class Shader {
		private:
			static void(*funifFuncs[5])(GLint, GLsizei, GLfloat*);
			static void(*iunifFuncs[5])(GLint, GLsizei, GLint*);

			GLuint id;

			std::unordered_map<std::string, unsigned int> uniformCache;

			bool checkErrors(GLuint shader, std::string type);
			void cacheUniformsLocations();

		public:
			static void funcInit();

			Shader(const std::string &vertexCode, const std::string &fragmentCode, const std::string &geometryCode = "");
			~Shader();

			void bind() const;

			void setUBOIndex(const std::string &name, unsigned int index);

			unsigned int getUniformId(const std::string &name) const;

			void send(const unsigned int &uid, const int &value) const;
			void send(const unsigned int &uid, const float &value) const;
			void send(const unsigned int &uid, const glm::vec2 &value) const;
			void send(const unsigned int &uid, const glm::vec3 &value) const;
			void send(const unsigned int &uid, const std::vector<glm::vec3> &values) const;
			void send(const unsigned int &uid, const glm::mat4 &mat) const;
			void send(const unsigned int &uid, const bool &value) const;

			void send(const std::string &name, const int &value) const;
			void send(const std::string &name, const float &value) const;
			void send(const std::string &name, const std::vector<float> &values) const;
			void send(const std::string &name, const glm::vec2 &value) const;
			void send(const std::string &name, const glm::vec3 &value) const;
			void send(const std::string &name, const std::vector<glm::vec3> &values) const;
			void send(const std::string &name, const glm::mat4 &mat) const;
			void send(const std::string &name, const bool &value) const;

			void sendRawFloat(const unsigned int &uid, const void *values, const int &funcId) const;
			void sendRawInt(const unsigned int &uid, const void *values, const int &funcId) const;
	};
}
