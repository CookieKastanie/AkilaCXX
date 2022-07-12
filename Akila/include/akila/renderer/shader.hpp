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
		std::size_t byteOffset; // position dans la memoire
		SendFunction sendFunctionPointer; // adresse de la fonction opengl correspondante
	};

	class Shader {
	public:
		Shader();
		Shader(std::string const &vertexTxt, std::string const &fragmentTxt, std::string const &geometryTxt = "");
		Shader(std::string const &shaderTxt);
		~Shader();

		void bind() const;
		bool isBinded() const;

		bool uniformExist(std::string const &name);
		UniformInfos &getUniforminfos(std::string const &name);

		std::size_t getTotalByteCount();

		void sendRaw(UniformInfos const &infos, void *data);
		void sendRaw(std::string const &name, void *data);

		bool readInt(std::string const &name, int *value);

	private:
		static GLuint bindedId;

		GLuint id;

		std::unordered_map<std::string, UniformInfos> uniformBindings;
		std::size_t totalByteCount;

		void build(std::string const &vertexTxt, std::string const &fragmentTxt, std::string const &geometryTxt = "");
		void cacheUniformsLocations();
	};
}
