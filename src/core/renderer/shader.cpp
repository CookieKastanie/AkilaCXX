#include "akila/core/renderer/shader.hpp"
#include "akila/core/renderer/shader_preproc.hpp"
#include <iostream>

using namespace akila;

GLuint Shader::bindedId = 0;

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

Shader::Shader(): id{0}, totalByteCount{0} {
	std::string prog = R"---(
		#akila_vertex
		out vec2 texCoord;
		void main() {
			texCoord.x = (gl_VertexID == 1) ? 2.0 : 0.0;
			texCoord.y = (gl_VertexID == 2) ? 2.0 : 0.0;
			gl_Position = vec4(texCoord * vec2(2.0) + vec2(-1.0), 1.0, 1.0);
		}
		#akila_fragment
		in vec2 texCoord;
		out vec4 fragColor;
		void main() {
			fragColor = vec4(texCoord, 0.0, 1.0);
		}
	)---";

	ShaderPreproc::ShaderSources sources;
	ShaderPreproc::process(prog, sources);
	build(sources.vertexShader, sources.fragmentShader, sources.geometryShader);
}

Shader::Shader(std::string const &vertexTxt, std::string const &fragmentTxt, std::string const &geometryTxt):
	id{0}, totalByteCount{0} {
	build(vertexTxt, fragmentTxt, geometryTxt);
}

Shader::Shader(std::string const &shaderTxt):
	id{0}, totalByteCount{0} {

	ShaderPreproc::ShaderSources sources;
	ShaderPreproc::process(shaderTxt, sources);
	build(sources.vertexShader, sources.fragmentShader, sources.geometryShader);
	templateFileName = sources.templateName;
}

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

Shader::Shader(Shader &&other) noexcept:
	id{other.id},
	uniformBindings{other.uniformBindings},
	totalByteCount{other.totalByteCount},
	fileName{other.fileName},
	templateFileName{other.templateFileName} {

	other.id = 0;
}

Shader &Shader::operator=(Shader &&other) noexcept {
	id = other.id;
	uniformBindings = other.uniformBindings;
	totalByteCount = other.totalByteCount;
	fileName = other.fileName;
	templateFileName = other.templateFileName;

	other.id = 0;

	return *this;
}

void Shader::bind() const {
	glUseProgram(id);
	bindedId = id;
}

bool Shader::isBinded() const {
	return bindedId == id;
}

void Shader::send(std::string const &name, int value) const {
	UniformInfos const &infos = uniformBindings.at(name);
	glProgramUniform1i(id, infos.location, value);
}

void Shader::send(std::string const &name, float value) const {
	UniformInfos const &infos = uniformBindings.at(name);
	glProgramUniform1f(id, infos.location, value);
}

void Shader::send(std::string const &name, std::vector<float> const &values) const {
	UniformInfos const &infos = uniformBindings.at(name);
	glProgramUniform1fv(id, infos.location, (GLsizei)values.size(), (GLfloat *)values.data());
}

void Shader::send(std::string const &name, Vec2 const &value) const {
	UniformInfos const &infos = uniformBindings.at(name);
	glProgramUniform2fv(id, infos.location, 1, &value[0]);
}

void Shader::send(std::string const &name, Vec3 const &value) const {
	UniformInfos const &infos = uniformBindings.at(name);
	glProgramUniform3fv(id, infos.location, 1, &value[0]);
}

void Shader::send(std::string const &name, std::vector<Vec3> const &values) const {
	UniformInfos const &infos = uniformBindings.at(name);
	glProgramUniform3fv(id, infos.location, (GLsizei)values.size(), (GLfloat *)values.data());
}

void Shader::send(std::string const &name, Mat4 const &mat) const {
	UniformInfos const &infos = uniformBindings.at(name);
	glProgramUniformMatrix4fv(id, infos.location, 1, GL_FALSE, &mat[0][0]);
}

void Shader::send(std::string const &name, bool value) const {
	UniformInfos const &infos = uniformBindings.at(name);
	glProgramUniform1i(id, infos.location, value);
}

void Shader::cacheUniformsLocations() {
	GLint size; // size of the variable (array length)
	GLenum type; // (float, vec3 or mat4, etc)

	GLsizei const bufSize = 128; //GL_UNIFORM_NAME_LENGTH stupidement trop grand
	GLchar name[bufSize];
	GLsizei length;

	GLint count;
	glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);

	uniformBindings.reserve(count);

	std::size_t byteOffset = 0;
	for(GLuint i = 0; i < static_cast<GLuint>(count); ++i) {
		glGetActiveUniform(id, i, bufSize, &length, &size, &type, name);

		unsigned int location = glGetUniformLocation(id, name); // a cause de la suppression des uniforms inutilises,
																// il faut query le shader pour avoir la vraie location
		if(location != -1) {
			UniformInfos &infos = uniformBindings[name];

			infos.location = location;
			infos.length = size;
			setUniformInfos(type, infos);
			infos.byteOffset = byteOffset;
			byteOffset += infos.byteCount;
		}
	}

	totalByteCount = byteOffset;
}

bool Shader::uniformExist(std::string const &name) const {
	return uniformBindings.find(name) != uniformBindings.end();
}

UniformInfos const &Shader::getUniforminfos(std::string const &name) const {
	return uniformBindings.at(name);
}

std::size_t Shader::getTotalByteCount() const {
	return totalByteCount;
}

void Shader::sendRaw(UniformInfos const &infos, void *data) const {
	infos.sendFunctionPointer(id, infos.location, infos.length, data);
}

void Shader::sendRaw(std::string const &name, void *data) const {
	UniformInfos const &infos = uniformBindings.at(name);
	infos.sendFunctionPointer(id, infos.location, infos.length, data);
}

bool Shader::readInt(std::string const &name, int *value) const {
	auto it = uniformBindings.find(name);
	if(it == uniformBindings.end()) return false;

	UniformInfos const &infos = it->second;
	if(infos.baseType != UniformUnderlyingType::INT
		&& infos.baseType != UniformUnderlyingType::SAMPLER) return false;

	glGetUniformiv(id, infos.location, value);
	return true;
}

void Shader::setFileName(std::string const &name) {
	fileName = name;
}

std::string const &Shader::getFileName() const {
	return fileName;
}

std::string const &Shader::getTemplateFileName() const {
	return templateFileName;
}
