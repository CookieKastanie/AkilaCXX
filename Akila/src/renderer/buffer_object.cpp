#include "akila/renderer/buffer_object.hpp"

using namespace akila;

BufferObject::BufferObject(unsigned int kind, Usage usage): kind{kind}, length{-1},
usage{static_cast<GLenum>(usage)}, dataType{static_cast<GLenum>(Type::FLOAT)} {
	glGenBuffers(1, &id);
}

BufferObject::~BufferObject() {
	glDeleteBuffers(1, &id);
}

void BufferObject::bind() const {
	glBindBuffer(kind, id);
}

void BufferObject::setRawData(void const *data, int size, int offset, int typeSize) {
	bind();
	if(length == -1) {
		glBufferData(kind, size, data, usage);
		length = size / typeSize;
	} else {
		glBufferSubData(kind, offset, size, data);
	}
}

int BufferObject::getLength() const {
	return length;
}

void BufferObject::setDataType(Type type) {
	dataType = static_cast<GLenum>(type);
}

BufferObject::Type BufferObject::getDataType() const {
	return static_cast<Type>(dataType);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

VBO::VBO(int tupleSize, unsigned int attributeLocation, Usage usage): BufferObject{GL_ARRAY_BUFFER, usage},
tupleSize{tupleSize}, location{attributeLocation} {

}

template<typename T>
void VBO::setData(std::vector<T> const &data) {
	setRawData(data.data(), (int)data.size() * sizeof(T));
	length = (int)data.size();
}

// declaration de tous les templates possibles
template void VBO::setData(std::vector<float> const &data);
template void VBO::setData(std::vector<glm::vec2> const &data);
template void VBO::setData(std::vector<glm::vec3> const &data);
template void VBO::setData(std::vector<glm::vec4> const &data);

int VBO::getTupleSize() const {
	return tupleSize;
}

unsigned int VBO::getLocation() const {
	return location;
}

void VBO::bindToArrayBuffer() const {
	bind();
	glVertexAttribPointer(getLocation(), getTupleSize(), static_cast<GLenum>(dataType), GL_FALSE, 0, 0);
	glEnableVertexAttribArray(getLocation());
}

/////////////////////////////////////////////////////////////////////////////////////////////////

IBO::IBO(Usage usage): BufferObject{GL_ELEMENT_ARRAY_BUFFER, usage} {
	setDataType(Type::UNSIGNED_SHORT);
}

template<typename T>
void IBO::setData(std::vector<T> const &data) {
	setRawData(data.data(), (int)data.size() * sizeof(T));
	length = (int)data.size();
}

// declaration de tous les templates possibles
template void IBO::setData(std::vector<unsigned short> const &data);
template void IBO::setData(std::vector<unsigned int> const &data);

/////////////////////////////////////////////////////////////////////////////////////////////////

unsigned int UBO::nextBindingPoint = 0;

UBO::UBO(unsigned int size, BufferObject::Usage usage): BufferObject{GL_UNIFORM_BUFFER, usage} {
	setRawData(NULL, size);
	length = size;
	bindingPoint = nextBindingPoint++;
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, id);
}

void UBO::setData(void const *data) {
	setRawData(data, length);
}

unsigned int UBO::getBindingPoint() {
	return bindingPoint;
}
