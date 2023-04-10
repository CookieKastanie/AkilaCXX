#include "akila/core/rhi/vao.hpp"

using namespace akila;

VAO::VAO(): drawMode{static_cast<GLuint>(Mode::TRIANGLES)}, length{0}, useIndices{false} {
	glGenVertexArrays(1, &id);
	drawFunc = drawArray;
	drawInstancedFunc = drawInstancedArray;
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
	drawFunc{other.drawFunc},
	drawInstancedFunc{other.drawInstancedFunc} {

	other.id = 0;
}

VAO &VAO::operator=(VAO &&other) noexcept {
	id = other.id;
	drawMode = other.drawMode;
	length = other.length;
	useIndices = other.useIndices;
	indicesType = other.indicesType;
	drawFunc = other.drawFunc;
	drawInstancedFunc = other.drawInstancedFunc;

	other.id = 0;

	return *this;
}

void VAO::setDrawMode(Mode mode) {
	drawMode = static_cast<GLuint>(mode);
}

void VAO::bind() const {
	glBindVertexArray(id);
}

void VAO::registerVBO(VBO const *vbo, unsigned int divisor) {
	vbo->bindToArrayBuffer(divisor);

	if(divisor != 0) return;
	if(useIndices) return;
	if(length >= vbo->getLength()) return;

	length = vbo->getLength();
}

void VAO::registerIBO(IBO const *ibo) {
	ibo->bind();
	useIndices = true;
	length = ibo->getLength();
	indicesType = static_cast<GLenum>(ibo->getDataType());
	drawFunc = drawElements;
	drawInstancedFunc = drawInstancedElements;
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

void VAO::drawInstancedArray(VAO const &vao, int instanceCount, int length) {
	vao.bind();
	glDrawArraysInstanced(vao.drawMode, 0, length, instanceCount);
	vao.unbind();
}

void VAO::drawInstancedElements(VAO const &vao, int instanceCount, int length) {
	vao.bind();
	glDrawElementsInstanced(vao.drawMode, length, vao.indicesType, nullptr, instanceCount);
	vao.unbind();
}

void VAO::draw() const {
	drawFunc(*this, length);
}

void VAO::draw(int overridedlength) const {
	drawFunc(*this, overridedlength);
}

void VAO::drawInstanced(int instanceCount) const {
	drawInstancedFunc(*this, instanceCount, length);
}

void VAO::drawInstanced(int instanceCount, int overridedlength) const {
	drawInstancedFunc(*this, instanceCount, overridedlength);
}
