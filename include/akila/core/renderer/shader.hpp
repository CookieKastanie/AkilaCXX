#pragma once

#include <glad/glad.h>
#include <string>
#include <unordered_map>
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

		Shader(Shader &&other) noexcept;
		Shader &operator=(Shader &&other) noexcept;

		Shader(Shader const &other) = delete;
		Shader &operator=(Shader const &other) = delete;

		void bind() const;
		bool isBinded() const;                                              

		void send(std::string const &name, int value) const;
		void send(std::string const &name, float value) const;
		void send(std::string const &name, std::vector<float> const &values) const;
		void send(std::string const &name, Vec2 const &value) const;
		void send(std::string const &name, Vec3 const &value) const;
		void send(std::string const &name, std::vector<Vec3> const &values) const;
		void send(std::string const &name, Mat4 const &mat) const;
		void send(std::string const &name, bool value) const;

		bool uniformExist(std::string const &name) const;
		UniformInfos const &getUniforminfos(std::string const &name) const;

		std::size_t getTotalByteCount() const;

		void sendRaw(UniformInfos const &infos, void *data) const;
		void sendRaw(std::string const &name, void *data) const;

		bool readInt(std::string const &name, int *value) const;

		void setFileName(std::string const &name);

		std::string const &getFileName() const;
		std::string const &getTemplateFileName() const;

	private:
		friend class Renderer;

		static GLuint bindedId;

		GLuint id;

		std::unordered_map<std::string, UniformInfos> uniformBindings;
		std::size_t totalByteCount;

		std::string fileName;
		std::string templateFileName;

		void build(std::string const &vertexTxt, std::string const &fragmentTxt, std::string const &geometryTxt = "");
		void cacheUniformsLocations();
	};
}
