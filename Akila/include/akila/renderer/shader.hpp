#pragma once

#include <glad/glad.h>
#include <string>
#include <unordered_map>

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

	using SendFunction = void(*)(GLint, GLsizei, void *);
	struct UniformInfos {
		unsigned int location; // adresse dans le shader
		UniformUnderlyingType baseType;
		int blockSize; // 1, 2, 3, ..., 16 pour scalair, vecteur, matrice
		int length; // nombre d'element dans l'array
		std::size_t byteCount; // nombre total d'octets
		SendFunction sendFunctionPointer; // adresse de la fonction opengl correspondante
	};

	class Shader {
	public:
		Shader();
		~Shader();

		void build(std::string const &vertexTxt, std::string const &fragmentTxt, std::string const &geometryTxt = "");

		void bind() const;

		void sendRaw(UniformInfos const &infos, void *data);

	//private:
		GLuint id;

		std::unordered_map<std::string, UniformInfos> uniformBindings;

		void cacheUniformsLocations();
	};
}
