#include "akila/renderer/vao.hpp"

using namespace akila;

VAO::VAO(): drawMode{static_cast<GLuint>(Mode::TRIANGLES)}, length{0}, useIndices{false} {
	glGenVertexArrays(1, &id);
	drawFunc = drawArray;
}

VAO::~VAO() {
	glDeleteVertexArrays(1, &id);
}

void VAO::setDrawMode(Mode mode) {
	drawMode = static_cast<GLuint>(mode);
}

void VAO::bind() const {
	glBindVertexArray(id);
}

void VAO::registerVBO(VBO const *vbo) {
	vbo->bindToArrayBuffer();
	if(!useIndices && length < vbo->getLength()) length = vbo->getLength();
}

void VAO::registerIBO(IBO const *ibo) {
	ibo->bind();
	useIndices = true;
	length = ibo->getLength();
	indicesType = static_cast<GLenum>(ibo->getDataType());
	drawFunc = drawElements;
}

void VAO::unbind() const {
	glBindVertexArray(0);
}

void VAO::drawArray(VAO const &vao) {
	vao.bind();
	glDrawArrays(vao.drawMode, 0, vao.length);
	vao.unbind();
}

void VAO::drawElements(VAO const &vao) {
	vao.bind();
	glDrawElements(vao.drawMode, vao.length, vao.indicesType, nullptr);
	vao.unbind();
}

void VAO::draw() const {
	drawFunc(*this);
}
