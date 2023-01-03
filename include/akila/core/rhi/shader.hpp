#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
#include "akila/core/math/math.hpp"

namespace akila {
	class Shader;
	// types de uniforms : 
	// scalar, vector, matrix, opaque (sampler, image, atomic)
	// https://www.khronos.org/opengl/wiki/Data_Type_(GLSL)

	enum class UniformUnderlyingType: unsigned int {
		FLOAT,
		INT,
		UINT,
		BOOL,
		SAMPLER
	};

	using SendFunction = void(*)(GLint, GLint, GLsizei, void *);
	struct UniformInfos {
		std::string name = ""; // nom de la variable
		unsigned int location = 0; // adresse dans le shader
		UniformUnderlyingType baseType = UniformUnderlyingType::FLOAT;
		int blockSize = 0; // 1, 2, 3, ..., 16 pour scalair, vecteur, matrice
		int length = 0; // nombre d'element dans l'array
		std::size_t byteCount = 0; // nombre total d'octets
		std::size_t byteOffset = 0; // position dans la memoire
		SendFunction sendFunctionPointer = nullptr; // adresse de la fonction opengl correspondante
	};

	class Shader {
	public:
		Shader(
			std::string const &vertexTxt = "",
			std::string const &geometryTxt = "",
			std::string const &fragmentTxt = ""
		);
		~Shader();

		Shader(Shader &&other) noexcept;
		Shader &operator=(Shader &&other) noexcept;

		Shader(Shader const &other) = delete;
		Shader &operator=(Shader const &other) = delete;

		void bind() const;

		std::vector<UniformInfos> retreiveUniformInfos();

		void send(UniformInfos const &infos, void const *data) const;
		bool readInt(UniformInfos const &infos, int *value) const;

	private:
		GLuint id;

		void build(std::string const &vertexTxt, std::string const &fragmentTxt, std::string const &geometryTxt = "");
	};
}
