#include "Akila/graphics/gl/Shader.hpp"

using namespace Akila;

void(*Shader::funifFuncs[5])(GLint, GLsizei, GLfloat *);
void(*Shader::iunifFuncs[5])(GLint, GLsizei, GLint *);

void Shader::funcInit() {
    funifFuncs[0] = (void(*)(GLint, GLsizei, GLfloat *))glUniform1fv;////

    funifFuncs[1] = (void(*)(GLint, GLsizei, GLfloat *))glUniform1fv;
    funifFuncs[2] = (void(*)(GLint, GLsizei, GLfloat *))glUniform2fv;
    funifFuncs[3] = (void(*)(GLint, GLsizei, GLfloat *))glUniform3fv;
    funifFuncs[4] = (void(*)(GLint, GLsizei, GLfloat *))glUniform4fv;


    iunifFuncs[0] = (void(*)(GLint, GLsizei, GLint *))glUniform1iv;/////

    iunifFuncs[1] = (void(*)(GLint, GLsizei, GLint *))glUniform1iv;
    iunifFuncs[2] = (void(*)(GLint, GLsizei, GLint *))glUniform2iv;
    iunifFuncs[3] = (void(*)(GLint, GLsizei, GLint *))glUniform3iv;
    iunifFuncs[4] = (void(*)(GLint, GLsizei, GLint *))glUniform4iv;
}


Shader::Shader(const std::string &vertexCode, const std::string &fragmentCode, const std::string &geometryCode) {
    id = glCreateProgram();

    unsigned int vertex, fragment, geometry = 0;
    // vertex shader
    {
        vertex = glCreateShader(GL_VERTEX_SHADER);
        GLchar const *c[] = {vertexCode.c_str()};
        glShaderSource(vertex, 1, c, NULL);
        glCompileShader(vertex);
        if(checkErrors(vertex, "VERTEX")) return;
        glAttachShader(id, vertex);
    }

    // fragment Shader
    {
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        GLchar const *c[] = {fragmentCode.c_str()};
        glShaderSource(fragment, 1, c, NULL);
        glCompileShader(fragment);
        if(checkErrors(fragment, "FRAGMENT")) return;
        glAttachShader(id, fragment);
    }

    // geometry Shader
    if(!geometryCode.empty()) {
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        GLchar const *c[] = {geometryCode.c_str()};
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
    if(!geometryCode.empty()) glDeleteShader(geometry);

    cacheUniformsLocations();
}

Shader::~Shader() {
    glDeleteProgram(id);
}

bool Shader::checkErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if(type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << std::endl << infoLog << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << std::endl << infoLog << std::endl;
        }
    }

    return !success;
}

void Shader::cacheUniformsLocations() {
    bind();

    GLint size; // size of the variable (array length)
    GLenum type; // (float, vec3 or mat4, etc)

    const GLsizei bufSize = 64;
    GLchar name[bufSize];
    GLsizei length;

    GLint count;
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);

    for(GLint i = 0; i < count; ++i) {
        glGetActiveUniform(id, (GLuint)i, bufSize, &length, &size, &type, name);

        unsigned int location = glGetUniformLocation(id, name); // a cause de la suppression des uniforms inutilises,
                                                                // il faut query le shader pour avoir la vraie location
        if(location != -1) {
            uniformCache[name] = location;
        }
    }
}

void Shader::bind() const {
    glUseProgram(id);
}

void Shader::setUBOIndex(const std::string &name, unsigned int index) {
    GLuint uniformBlock = glGetUniformBlockIndex(id, name.c_str());
    glUniformBlockBinding(id, uniformBlock, index);
}

unsigned int Shader::getUniformId(const std::string &name) const {
    auto &it = uniformCache.find(name);
    if(it == uniformCache.end()) return -1;
    else return it->second;
}


/////////////////////////////////


void Shader::send(const unsigned int &uid, const int &value) const {
    glUniform1i(uid, value);
}

void Shader::send(const unsigned int &uid, const float &value) const {
    glUniform1f(uid, value);
}

void Shader::send(const unsigned int &uid, const glm::vec2 &value) const {
    glUniform2fv(uid, 1, &value[0]);
}

void Shader::send(const unsigned int &uid, const glm::vec3 &value) const {
    glUniform3fv(uid, 1, &value[0]);
}

void Shader::send(const unsigned int &uid, const std::vector<glm::vec3> &values) const {
    glUniform3fv(uid, (GLsizei)values.size(), (GLfloat*)values.data());
}

void Shader::send(const unsigned int &uid, const glm::mat4 &mat) const {
    glUniformMatrix4fv(uid, 1, GL_FALSE, &mat[0][0]);
}

void Shader::send(const unsigned int &uid, const bool &value) const {
    glUniform1i(uid, value);
}

/////////////////////////////////

void Shader::send(const std::string &name, const int &value) const {
    glUniform1i(uniformCache.find(name)->second, value);
}

void Shader::send(const std::string &name, const float &value) const {
    glUniform1f(uniformCache.find(name)->second, value);
}

void Shader::send(const std::string &name, const std::vector<float> &values) const {
    glUniform1fv(uniformCache.find(name)->second, (GLsizei)values.size(), (GLfloat *)values.data());
}

void Shader::send(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(uniformCache.find(name)->second, 1, &value[0]);
}

void Shader::send(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(uniformCache.find(name)->second, 1, &value[0]);
}

void Shader::send(const std::string &name, const std::vector<glm::vec3> &values) const {
    glUniform3fv(uniformCache.find(name)->second, (GLsizei)values.size(), (GLfloat *)values.data());
}

void Shader::send(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(uniformCache.find(name)->second, 1, GL_FALSE, &mat[0][0]);
}

void Shader::send(const std::string &name, const bool &value) const {
    glUniform1i(uniformCache.find(name)->second, value);
}

/////////////////////////////////

void Shader::sendRawFloat(const unsigned int &uid, const void *values, const int &funcId) const {
    funifFuncs[funcId](uid, 1, (GLfloat*)values);
}

void Shader::sendRawInt(const unsigned int &uid, const void *values, const int &funcId) const {
    iunifFuncs[funcId](uid, 1, (GLint*)values);
}
