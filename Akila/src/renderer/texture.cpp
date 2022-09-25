#include "akila/renderer/texture.hpp"

using namespace akila;

TextureBuffer::TextureBuffer(Kind kind, Format internalFormat):
	kind{static_cast<GLenum>(kind)},
	internalFormat{static_cast<GLenum>(internalFormat)},
	size{0, 0} {
	glGenTextures(1, &id);
	setParameters({});
}

TextureBuffer::~TextureBuffer() {
	glDeleteTextures(1, &id);
}

unsigned int TextureBuffer::getId() const {
	return id;
}

void TextureBuffer::bind(unsigned int unit) const {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(kind, id);
}

IVec2 TextureBuffer::getSize() const {
	return size;
}

TextureBuffer::Parameters::Parameters():
	wrapS{WrapMode::REPEAT},
	wrapT{WrapMode::REPEAT},
	wrapR{WrapMode::REPEAT},
	minFilter{FilterMode::LINEAR},
	magFilter{FilterMode::LINEAR} {}

void TextureBuffer::setParameters(Parameters const &params) {
	bind();

	glTexParameteri(kind, GL_TEXTURE_WRAP_S, static_cast<GLint>(params.wrapS));
	glTexParameteri(kind, GL_TEXTURE_WRAP_T, static_cast<GLint>(params.wrapT));
	glTexParameteri(kind, GL_TEXTURE_WRAP_R, static_cast<GLint>(params.wrapR));
	glTexParameteri(kind, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(params.minFilter));
	glTexParameteri(kind, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(params.magFilter));
}

TextureBuffer::Format TextureBuffer::getInternalFormat() {
	return (Format)internalFormat;
}

void TextureBuffer::generateMipmap() {
	bind();
	glGenerateMipmap(kind);
}

/////

Texture2D::Texture2D(Format format): TextureBuffer{Kind::TEXTURE_2D, format} {}

void Texture2D::setSize(IVec2 size) {
	bind();

	glTexImage2D(
		kind, 0, internalFormat, size.x, size.y, 0,
		internalFormat == GL_DEPTH_COMPONENT ?
		GL_DEPTH_COMPONENT : static_cast<GLenum>(Format::RGB),
		static_cast<GLenum>(Type::UNSIGNED_BYTE), nullptr
	);

	this->size = size;
}

void Texture2D::setData(void const *data, Format format, Type type, unsigned int mipLevel) {
	bind();
	glTexSubImage2D(
		kind, mipLevel, 0, 0, size.x, size.y,
		static_cast<GLenum>(format), static_cast<GLenum>(type), data
	);
}
