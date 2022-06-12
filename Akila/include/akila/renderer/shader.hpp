#pragma once

#include <glad/glad.h>
#include <string>
#include <unordered_map>

namespace akila {
	enum class UniformBaseType: unsigned int {
		FLOAT = 0,
		INT = 1,
		UINT = 2,
		BOOL = 3
	};

	class Shader;

	struct Uniform {
		Shader *shader;
		std::string name;
		unsigned int location;
		UniformBaseType baseType;
		int vec;
		int count;
	};

	class Shader {
	public:
		Shader(std::string const &vertexTxt, std::string const &fragmentTxt, std::string const &geometryTxt = "");
		~Shader();

		void bind() const;

	private:
		GLuint id;
		std::unordered_map<std::string, Uniform> uniforms;

		void cacheUniformsLocations();
	};
}
