#include "Akila/graphics/gl/Buffer.hpp"

using namespace Akila;

Buffer::Buffer(unsigned int kind, unsigned int usage): kind{kind}, length{-1}, usage{usage} {
	glGenBuffers(1, &id);
}

Buffer::~Buffer() {
	glDeleteBuffers(1, &id);
}

void Buffer::bind() const {
	glBindBuffer(kind, id);
}

void Buffer::setRawData(const void *data, int size, int offset) {
	bind();
	if(length == -1) glBufferData(kind, size, data, usage);
	else glBufferSubData(kind, offset, size, data);
}

int Buffer::getLength() const {
	return length;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

VBO::VBO(int tupleSize, unsigned int attributeLocation, unsigned int usage): Buffer{GL_ARRAY_BUFFER, usage},
	tupleSize{tupleSize}, location{attributeLocation} {
	
}

template<typename T>
void VBO::setData(const std::vector<T> &data) {
	setRawData(data.data(), data.size() * sizeof(T));
	length = data.size();
}

// declaration de tous les templates possibles
template void VBO::setData(const std::vector<float> &data);
template void VBO::setData(const std::vector<glm::vec2> &data);
template void VBO::setData(const std::vector<glm::vec3> &data);
template void VBO::setData(const std::vector<glm::vec4> &data);

int VBO::getTupleSize() const {
	return tupleSize;
}

unsigned int VBO::getLocation() const {
	return location;
}

void VBO::bindToArrayBuffer(unsigned int dataType) const {
	bind();
	glVertexAttribPointer(getLocation(), getTupleSize(), dataType, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(getLocation());
}

/////////////////////////////////////////////////////////////////////////////////////////////////

UBO::UBO(unsigned int size, unsigned int usage): Buffer{GL_UNIFORM_BUFFER, usage} {
	setRawData(NULL, size);
	length = size;
}

void UBO::setData(const void *data) {
	setRawData(data, length);
}
