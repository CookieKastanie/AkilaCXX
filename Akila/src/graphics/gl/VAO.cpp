#include "Akila/graphics/gl/VAO.hpp"

using namespace Akila;

VAO::VAO(): drawMode{TRIANGLES}, length {0} {
	glGenVertexArrays(1, &id);
}

VAO::~VAO() {
	glDeleteVertexArrays(1, &id);
}

void VAO::setDrawMode(unsigned int mode) {
	drawMode = mode;
}

void VAO::bind() const {
	glBindVertexArray(id);
}

void VAO::registerVBO(const VBO *vbo) {
	vbo->bindToArrayBuffer();
	if(length < vbo->getLength()) length = vbo->getLength();
}

void VAO::unbind() const {
	glBindVertexArray(0);
}

void VAO::draw() const {
	bind();
	glDrawArrays(drawMode, 0, length);
	unbind();
}
