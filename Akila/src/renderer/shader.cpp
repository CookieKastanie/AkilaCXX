#include "akila/renderer/shader.hpp"
#include <iostream>

using namespace akila;

void(*internal::GL_FUNC_ARRAY::funifFuncs[5])(GLint, GLsizei, GLfloat*);
void(*internal::GL_FUNC_ARRAY::iunifFuncs[5])(GLint, GLsizei, GLint*);

void internal::GL_FUNC_ARRAY::init() {
	funifFuncs[1] = (void(*)(GLint, GLsizei, GLfloat*))glUniform1fv;
	funifFuncs[2] = (void(*)(GLint, GLsizei, GLfloat*))glUniform2fv;
	funifFuncs[3] = (void(*)(GLint, GLsizei, GLfloat*))glUniform3fv;
	funifFuncs[4] = (void(*)(GLint, GLsizei, GLfloat*))glUniform4fv;

	iunifFuncs[1] = (void(*)(GLint, GLsizei, GLint*))glUniform1iv;
	iunifFuncs[2] = (void(*)(GLint, GLsizei, GLint*))glUniform2iv;
	iunifFuncs[3] = (void(*)(GLint, GLsizei, GLint*))glUniform3iv;
	iunifFuncs[4] = (void(*)(GLint, GLsizei, GLint*))glUniform4iv;
}

void glTypeToSizeAndBaseType(GLenum glType, int &size, UniformBaseType &baseType) {
	switch(glType) {
		case GL_FLOAT: size = 1; baseType = UniformBaseType::FLOAT; break;
		case GL_FLOAT_VEC2: size = 2; baseType = UniformBaseType::FLOAT; break;
		case GL_FLOAT_VEC3: size = 3; baseType = UniformBaseType::FLOAT; break;
		case GL_FLOAT_VEC4: size = 4; baseType = UniformBaseType::FLOAT; break;

		case GL_INT: size = 1; baseType = UniformBaseType::INT; break;
		case GL_INT_VEC2: size = 2; baseType = UniformBaseType::INT; break;
		case GL_INT_VEC3: size = 3; baseType = UniformBaseType::INT; break;
		case GL_INT_VEC4: size = 4; baseType = UniformBaseType::INT; break;

		case GL_UNSIGNED_INT: size = 1; baseType = UniformBaseType::UINT; break;
		case GL_UNSIGNED_INT_VEC2: size = 2; baseType = UniformBaseType::UINT; break;
		case GL_UNSIGNED_INT_VEC3: size = 3; baseType = UniformBaseType::UINT; break;
		case GL_UNSIGNED_INT_VEC4: size = 4; baseType = UniformBaseType::UINT; break;

		case GL_BOOL: size = 1; baseType = UniformBaseType::BOOL; break;
		case GL_BOOL_VEC2: size = 2; baseType = UniformBaseType::BOOL; break;
		case GL_BOOL_VEC3: size = 3; baseType = UniformBaseType::BOOL; break;
		case GL_BOOL_VEC4: size = 4; baseType = UniformBaseType::BOOL; break;

		default: size = 1; baseType = UniformBaseType::INT; break;
	}
}

bool checkErrors(GLuint shader, std::string type) {
	GLint success;
	GLchar infoLog[1024];
	if(type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << std::endl << infoLog << std::endl;
		}
	} else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if(!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << std::endl << infoLog << std::endl;
		}
	}

	return !success;
}

Shader::Shader(): id{0} {}

void Shader::build(std::string const &vertexTxt, std::string const &fragmentTxt, std::string const &geometryTxt) {
	if(id != 0) {
		std::cerr << "Shader already built !" << std::endl;
	}

	std::cout << vertexTxt << std::endl;

	std::cout << fragmentTxt << std::endl;

	id = glCreateProgram();
	
	unsigned int vertex, fragment, geometry = 0;
	// vertex shader
	{
		vertex = glCreateShader(GL_VERTEX_SHADER);
		GLchar const *c[] = {vertexTxt.c_str()};
		glShaderSource(vertex, 1, c, NULL);
		glCompileShader(vertex);
		if(checkErrors(vertex, "VERTEX")) return;
		glAttachShader(id, vertex);
	}

	// fragment Shader
	{
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		GLchar const *c[] = {fragmentTxt.c_str()};
		glShaderSource(fragment, 1, c, NULL);
		glCompileShader(fragment);
		if(checkErrors(fragment, "FRAGMENT")) return;
		glAttachShader(id, fragment);
	}

	// geometry Shader
	if(!geometryTxt.empty()) {
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		GLchar const *c[] = {geometryTxt.c_str()};
		glShaderSource(geometry, 1, c, NULL);
		glCompileShader(geometry);
		if(checkErrors(geometry, "GEOMETRY")) return;
		glAttachShader(id, geometry);
	}

	// shader Program
	glLinkProgram(id);
	checkErrors(id, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if(!geometryTxt.empty()) glDeleteShader(geometry);

	cacheUniformsLocations();
}

Shader::~Shader() {
	glDeleteProgram(id);
}

void Shader::bind() const {
	glUseProgram(id);
}

void Shader::cacheUniformsLocations() {
	bind();

	GLint size; // size of the variable (array length)
	GLenum type; // (float, vec3 or mat4, etc)

	GLsizei const bufSize = 128; //GL_UNIFORM_NAME_LENGTH stupidement trop grand
	GLchar name[bufSize];
	GLsizei length;

	GLint count;
	glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);

	uniformBindings.reserve(count);

	for(GLuint i = 0; i < static_cast<GLuint>(count); ++i) {
		glGetActiveUniform(id, i, bufSize, &length, &size, &type, name);

		unsigned int location = glGetUniformLocation(id, name); // a cause de la suppression des uniforms inutilises,
																// il faut query le shader pour avoir la vraie location
		if(location != -1) {
			UniformBaseType baseType;
			int vecSize;

			glTypeToSizeAndBaseType(type, vecSize, baseType);

			uniformBindings[name] = UniformInfos{
				location,
				baseType,
				vecSize,
				size
			};
		}
	}
}

void Shader::sendRawFloats(unsigned int uid, int size, int count, void* values) {
	internal::GL_FUNC_ARRAY::funifFuncs[size](uid, count, static_cast<GLfloat*>(values));
}

void Shader::sendRawInts(unsigned int uid, int size, int count, void* values) {
	internal::GL_FUNC_ARRAY::iunifFuncs[size](uid, count, static_cast<GLint*>(values));
}
