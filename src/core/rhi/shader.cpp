#include "akila/core/rhi/shader.hpp"
#include <iostream>

using namespace akila;

void override_glUniformMatrix2fv(GLint id, GLint l, GLsizei s, void *d) {
	glProgramUniformMatrix2fv(id, l, s, false, (GLfloat *)d);
}

void override_glUniformMatrix3fv(GLint id, GLint l, GLsizei s, void *d) {
	glProgramUniformMatrix3fv(id, l, s, false, (GLfloat *)d);
}

void override_glUniformMatrix4fv(GLint id, GLint l, GLsizei s, void *d) {
	glProgramUniformMatrix4fv(id, l, s, false, (GLfloat *)d);
}

void ssui(UniformInfos &infos, int size, UniformUnderlyingType baseType, std::size_t typeByte, SendFunction sendFunctionPointer) {
	infos.blockSize = size;
	infos.baseType = baseType;
	infos.byteCount = typeByte * infos.blockSize * infos.length;
	infos.sendFunctionPointer = sendFunctionPointer;
}

void setUniformInfos(GLenum glType, UniformInfos &infos) {
	switch(glType) { // quel enfer
		case GL_FLOAT:      ssui(infos, 1, UniformUnderlyingType::FLOAT, sizeof(GLfloat), (SendFunction)glProgramUniform1fv); break;
		case GL_FLOAT_VEC2: ssui(infos, 2, UniformUnderlyingType::FLOAT, sizeof(GLfloat), (SendFunction)glProgramUniform2fv); break;
		case GL_FLOAT_VEC3: ssui(infos, 3, UniformUnderlyingType::FLOAT, sizeof(GLfloat), (SendFunction)glProgramUniform3fv); break;
		case GL_FLOAT_VEC4: ssui(infos, 4, UniformUnderlyingType::FLOAT, sizeof(GLfloat), (SendFunction)glProgramUniform4fv); break;

		case GL_FLOAT_MAT2: ssui(infos,  4, UniformUnderlyingType::FLOAT, sizeof(GLfloat), override_glUniformMatrix2fv); break;
		case GL_FLOAT_MAT3: ssui(infos,  9, UniformUnderlyingType::FLOAT, sizeof(GLfloat), override_glUniformMatrix3fv); break;
		case GL_FLOAT_MAT4: ssui(infos, 16, UniformUnderlyingType::FLOAT, sizeof(GLfloat), override_glUniformMatrix4fv); break;

		case GL_INT:      ssui(infos, 1, UniformUnderlyingType::INT, sizeof(GLint), (SendFunction)glProgramUniform1iv); break;
		case GL_INT_VEC2: ssui(infos, 2, UniformUnderlyingType::INT, sizeof(GLint), (SendFunction)glProgramUniform2iv); break;
		case GL_INT_VEC3: ssui(infos, 3, UniformUnderlyingType::INT, sizeof(GLint), (SendFunction)glProgramUniform3iv); break;
		case GL_INT_VEC4: ssui(infos, 4, UniformUnderlyingType::INT, sizeof(GLint), (SendFunction)glProgramUniform4iv); break;

		case GL_UNSIGNED_INT:      ssui(infos, 1, UniformUnderlyingType::UINT, sizeof(GLuint), (SendFunction)glProgramUniform1uiv); break;
		case GL_UNSIGNED_INT_VEC2: ssui(infos, 2, UniformUnderlyingType::UINT, sizeof(GLuint), (SendFunction)glProgramUniform2uiv); break;
		case GL_UNSIGNED_INT_VEC3: ssui(infos, 3, UniformUnderlyingType::UINT, sizeof(GLuint), (SendFunction)glProgramUniform3uiv); break;
		case GL_UNSIGNED_INT_VEC4: ssui(infos, 4, UniformUnderlyingType::UINT, sizeof(GLuint), (SendFunction)glProgramUniform4uiv); break;

		case GL_BOOL:      ssui(infos, 1, UniformUnderlyingType::BOOL, sizeof(GLboolean), (SendFunction)glProgramUniform1iv); break;
		case GL_BOOL_VEC2: ssui(infos, 2, UniformUnderlyingType::BOOL, sizeof(GLboolean), (SendFunction)glProgramUniform2iv); break;
		case GL_BOOL_VEC3: ssui(infos, 3, UniformUnderlyingType::BOOL, sizeof(GLboolean), (SendFunction)glProgramUniform3iv); break;
		case GL_BOOL_VEC4: ssui(infos, 4, UniformUnderlyingType::BOOL, sizeof(GLboolean), (SendFunction)glProgramUniform4iv); break;

		default: ssui(infos, 1, UniformUnderlyingType::SAMPLER, sizeof(GLint), (SendFunction)glProgramUniform1iv); break;
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

/////////////////////////////////////////

Shader::Shader(): id{0} {}

Shader::Shader(std::string const &vertexTxt, std::string const &fragmentTxt, std::string const &geometryTxt):
	id{0} {
	build(vertexTxt, fragmentTxt, geometryTxt);
}

void Shader::build(std::string const &vertexTxt, std::string const &geometryTxt, std::string const &fragmentTxt) {
	if(id != 0) {
		std::cerr << "Shader already built !" << std::endl;
	}

	if(vertexTxt.empty() && geometryTxt.empty() && fragmentTxt.empty()) {
		return;
	}

	id = glCreateProgram();
	
	unsigned int vertex = 0, fragment = 0, geometry = 0;
	// vertex shader
	if(vertexTxt.empty() == false) {
		vertex = glCreateShader(GL_VERTEX_SHADER);
		GLchar const *c[] = {vertexTxt.c_str()};
		glShaderSource(vertex, 1, c, NULL);
		glCompileShader(vertex);
		if(checkErrors(vertex, "VERTEX")) return;
		glAttachShader(id, vertex);
	}

	// geometry Shader
	if(geometryTxt.empty() == false) {
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		GLchar const *c[] = {geometryTxt.c_str()};
		glShaderSource(geometry, 1, c, NULL);
		glCompileShader(geometry);
		if(checkErrors(geometry, "GEOMETRY")) return;
		glAttachShader(id, geometry);
	}

	// fragment Shader
	if(fragmentTxt.empty() == false) {
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		GLchar const *c[] = {fragmentTxt.c_str()};
		glShaderSource(fragment, 1, c, NULL);
		glCompileShader(fragment);
		if(checkErrors(fragment, "FRAGMENT")) return;
		glAttachShader(id, fragment);
	}

	// shader Program
	glLinkProgram(id);
	checkErrors(id, "PROGRAM");

	if(vertexTxt.empty() == false) glDeleteShader(vertex);
	if(geometryTxt.empty() == false) glDeleteShader(geometry);
	if(fragmentTxt.empty() == false) glDeleteShader(fragment);
}

Shader::~Shader() {
	glDeleteProgram(id);
}

Shader::Shader(Shader &&other) noexcept:
	id{other.id} {

	other.id = 0;
}

Shader &Shader::operator=(Shader &&other) noexcept {
	id = other.id;
	other.id = 0;

	return *this;
}

void Shader::bind() const {
	glUseProgram(id);
}

std::vector<UniformInfos> Shader::retreiveUniformInfos() {
	std::vector<UniformInfos> uniformBindings;

	GLint size; // size of the variable (array length)
	GLenum type; // (float, vec3 or mat4, etc)

	GLsizei const bufSize = 256; //GL_UNIFORM_NAME_LENGTH stupidement trop grand
	GLchar name[bufSize];
	GLsizei length;

	GLint count;
	glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);

	std::size_t byteOffset = 0;
	std::size_t index = 0;
	for(GLuint i = 0; i < static_cast<GLuint>(count); ++i) {
		glGetActiveUniform(id, i, bufSize, &length, &size, &type, name);

		// a cause de la suppression des uniforms inutilises,
		// il faut query le shader pour avoir la vraie location
		unsigned int location = glGetUniformLocation(id, name);

		if(location != -1) {
			UniformInfos infos;

			infos.name = name;
			infos.location = location;
			infos.length = size;
			setUniformInfos(type, infos);
			infos.byteOffset = byteOffset;
			byteOffset += infos.byteCount;

			uniformBindings.push_back(infos);
		}
	}

	return uniformBindings;
}

void Shader::send(UniformInfos const &infos, void const *data) const {
	infos.sendFunctionPointer(id, infos.location, infos.length, const_cast<void *>(data));
}

bool Shader::readInt(UniformInfos const &infos, int *value) const {
	if(infos.baseType != UniformUnderlyingType::INT
		&& infos.baseType != UniformUnderlyingType::SAMPLER) return false;

	glGetUniformiv(id, infos.location, value);
	return true;
}
