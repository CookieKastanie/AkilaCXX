#include "akila/renderer/shader.hpp"
#include <iostream>

using namespace akila;

void inline override_glUniformMatrix2fv(GLint l, GLsizei s, void *d) {
	glUniformMatrix2fv(l, s, false, (GLfloat *)d);
}

void inline override_glUniformMatrix3fv(GLint l, GLsizei s, void *d) {
	glUniformMatrix3fv(l, s, false, (GLfloat *)d);
}

void inline override_glUniformMatrix4fv(GLint l, GLsizei s, void *d) {
	glUniformMatrix4fv(l, s, false, (GLfloat *)d);
}

void inline ssui(UniformInfos &infos, int size, UniformUnderlyingType baseType, std::size_t typeByte, SendFunction sendFunctionPointer) {
	infos.blockSize = size;
	infos.baseType = baseType;
	infos.byteCount = typeByte * infos.blockSize * infos.length;
	infos.sendFunctionPointer = sendFunctionPointer;
}

void setUniformInfos(GLenum glType, UniformInfos &infos) {
	switch(glType) { // quel enfer
		case GL_FLOAT:      ssui(infos, 1, UniformUnderlyingType::FLOAT, sizeof(GLfloat), (SendFunction)glUniform1fv); break;
		case GL_FLOAT_VEC2: ssui(infos, 2, UniformUnderlyingType::FLOAT, sizeof(GLfloat), (SendFunction)glUniform2fv); break;
		case GL_FLOAT_VEC3: ssui(infos, 3, UniformUnderlyingType::FLOAT, sizeof(GLfloat), (SendFunction)glUniform3fv); break;
		case GL_FLOAT_VEC4: ssui(infos, 4, UniformUnderlyingType::FLOAT, sizeof(GLfloat), (SendFunction)glUniform4fv); break;

		case GL_FLOAT_MAT2: ssui(infos,  4, UniformUnderlyingType::FLOAT, sizeof(GLfloat), override_glUniformMatrix2fv); break;
		case GL_FLOAT_MAT3: ssui(infos,  9, UniformUnderlyingType::FLOAT, sizeof(GLfloat), override_glUniformMatrix3fv); break;
		case GL_FLOAT_MAT4: ssui(infos, 16, UniformUnderlyingType::FLOAT, sizeof(GLfloat), override_glUniformMatrix4fv); break;

		case GL_INT:      ssui(infos, 1, UniformUnderlyingType::INT, sizeof(GLint), (SendFunction)glUniform1iv); break;
		case GL_INT_VEC2: ssui(infos, 2, UniformUnderlyingType::INT, sizeof(GLint), (SendFunction)glUniform2iv); break;
		case GL_INT_VEC3: ssui(infos, 3, UniformUnderlyingType::INT, sizeof(GLint), (SendFunction)glUniform3iv); break;
		case GL_INT_VEC4: ssui(infos, 4, UniformUnderlyingType::INT, sizeof(GLint), (SendFunction)glUniform4iv); break;

		case GL_UNSIGNED_INT:      ssui(infos, 1, UniformUnderlyingType::UINT, sizeof(GLuint), (SendFunction)glUniform1uiv); break;
		case GL_UNSIGNED_INT_VEC2: ssui(infos, 2, UniformUnderlyingType::UINT, sizeof(GLuint), (SendFunction)glUniform2uiv); break;
		case GL_UNSIGNED_INT_VEC3: ssui(infos, 3, UniformUnderlyingType::UINT, sizeof(GLuint), (SendFunction)glUniform3uiv); break;
		case GL_UNSIGNED_INT_VEC4: ssui(infos, 4, UniformUnderlyingType::UINT, sizeof(GLuint), (SendFunction)glUniform4uiv); break;

		case GL_BOOL:      ssui(infos, 1, UniformUnderlyingType::BOOL, sizeof(GLboolean), (SendFunction)glUniform1iv); break;
		case GL_BOOL_VEC2: ssui(infos, 2, UniformUnderlyingType::BOOL, sizeof(GLboolean), (SendFunction)glUniform2iv); break;
		case GL_BOOL_VEC3: ssui(infos, 3, UniformUnderlyingType::BOOL, sizeof(GLboolean), (SendFunction)glUniform3iv); break;
		case GL_BOOL_VEC4: ssui(infos, 4, UniformUnderlyingType::BOOL, sizeof(GLboolean), (SendFunction)glUniform4iv); break;

		default: ssui(infos, 1, UniformUnderlyingType::SAMPLER, sizeof(GLint), (SendFunction)glUniform1iv); break;
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
			UniformInfos &infos = uniformBindings[name];
			infos.location = location;
			infos.length = size;
			setUniformInfos(type, infos);
		}
	}
}

void Shader::sendRaw(UniformInfos const &infos, void *data) {
	infos.sendFunctionPointer(infos.location, infos.length, data);
}

//void Shader::sendRawFloats(unsigned int uid, int size, int count, void* values) {
//	internal::GL_FUNC_ARRAY::funifFuncs[size](uid, count, static_cast<GLfloat*>(values));
//}

//void Shader::sendRawInts(unsigned int uid, int size, int count, void* values) {
//	internal::GL_FUNC_ARRAY::iunifFuncs[size](uid, count, static_cast<GLint*>(values));
//}
