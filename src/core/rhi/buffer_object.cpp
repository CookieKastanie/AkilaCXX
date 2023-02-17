#include "akila/core/rhi/buffer_object.hpp"

using namespace akila;

BufferObject::BufferObject(unsigned int kind, Usage usage): kind{kind}, length{-1},
usage{static_cast<GLenum>(usage)}, dataType{static_cast<GLenum>(Type::FLOAT)} {
	glGenBuffers(1, &id);
}

BufferObject::~BufferObject() {
	glDeleteBuffers(1, &id);
}

BufferObject::BufferObject(BufferObject &&other) noexcept:
	id{other.id},
	kind{other.kind},
	length{other.length},
	usage{other.usage},
	dataType{other.dataType} {

	other.id = 0;
}

BufferObject &BufferObject::operator=(BufferObject &&other) noexcept {
	id = other.id;
	kind = other.kind;
	length = other.length;
	usage = other.usage;
	dataType = other.dataType;

	other.id = 0;

	return *this;
}

void BufferObject::bind() const {
	glBindBuffer(kind, id);
}

void BufferObject::setRawData(
	void const *data,
	std::size_t size, std::size_t offset, std::size_t count) {

	bind();
	if(length == -1) {
		glBufferData(kind, size, data, usage);
		length = static_cast<int>(count);
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

VBO::VBO(VBO &&other) noexcept:
	BufferObject{std::move(other)},
	tupleSize{other.tupleSize},
	location{other.location} {

}

VBO &VBO::operator=(VBO &&other) noexcept {
	BufferObject::operator=(std::move(other));
	tupleSize = other.tupleSize;
	location = other.location;

	return *this;
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

UBO::UBO(unsigned int bindingPoint, unsigned int size, BufferObject::Usage usage): BufferObject{GL_UNIFORM_BUFFER, usage}, bindingPoint{bindingPoint} {
	setRawData(NULL, size);
	length = size;
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, id);
}

UBO::UBO(UBO &&other) noexcept:
	BufferObject{std::move(other)},
	bindingPoint{other.bindingPoint} {

}

UBO &UBO::operator=(UBO &&other) noexcept {
	BufferObject::operator=(std::move(other));
	bindingPoint = other.bindingPoint;

	return *this;
}

void UBO::setData(void const *data) {
	setRawData(data, length);
}

unsigned int UBO::getBindingPoint() {
	return bindingPoint;
}
