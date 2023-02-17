#include "akila/core/rhi/vao.hpp"

using namespace akila;

VAO::VAO(): drawMode{static_cast<GLuint>(Mode::TRIANGLES)}, length{0}, useIndices{false} {
	glGenVertexArrays(1, &id);
	drawFunc = drawArray;
}

VAO::~VAO() {
	glDeleteVertexArrays(1, &id);
}

VAO::VAO(VAO &&other) noexcept:
	id{other.id},
	drawMode{other.drawMode},
	length{other.length},
	useIndices{other.useIndices},
	indicesType{other.indicesType},
	drawFunc{other.drawFunc} {

	other.id = 0;
}

VAO &VAO::operator=(VAO &&other) noexcept {
	id = other.id;
	drawMode = other.drawMode;
	length = other.length;
	useIndices = other.useIndices;
	indicesType = other.indicesType;
	drawFunc = other.drawFunc;

	other.id = 0;

	return *this;
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

void VAO::drawArray(VAO const &vao, int length) {
	vao.bind();
	glDrawArrays(vao.drawMode, 0, length);
	vao.unbind();
}

void VAO::drawElements(VAO const &vao, int length) {
	vao.bind();
	glDrawElements(vao.drawMode, length, vao.indicesType, nullptr);
	vao.unbind();
}

void VAO::draw() const {
	drawFunc(*this, length);
}

void VAO::draw(int overridedlength) const {
	drawFunc(*this, overridedlength);
}
