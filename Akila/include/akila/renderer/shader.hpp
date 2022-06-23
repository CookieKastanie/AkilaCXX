#pragma once

#include <glad/glad.h>
#include <string>
#include <unordered_map>

namespace akila {
	class Shader;

	namespace internal {
		class GL_FUNC_ARRAY {
		private:
			friend class Window;
			friend class Shader;

			static void(*funifFuncs[5])(GLint, GLsizei, GLfloat*);
			static void(*iunifFuncs[5])(GLint, GLsizei, GLint*);

			static void init();
		};
	}

	enum class UniformBaseType: unsigned int {
		FLOAT = 0,
		INT = 1,
		UINT = 2,
		BOOL = 3
	};

	struct UniformBinding {
		unsigned int location;
		UniformBaseType baseType;
		int size; // vec1 vec2 vec3
		int count;
	};

	class Shader {
	public:
		Shader();
		~Shader();

		void build(std::string const &vertexTxt, std::string const &fragmentTxt, std::string const &geometryTxt = "");

		void bind() const;

		void sendRawFloats(unsigned int uid, int size, int count, void *values);
		void sendRawInts(unsigned int uid, int size, int count, void *values);

	private:
		GLuint id;
		std::unordered_map<std::string, UniformBinding> uniformBindings;

		void cacheUniformsLocations();
	};
}
