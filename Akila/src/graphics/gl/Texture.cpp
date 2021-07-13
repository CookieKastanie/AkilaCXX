#include "Akila/graphics/gl/Texture.hpp"

using namespace Akila;

TextureBuffer::TextureBuffer(unsigned int kind, Format internalFormat):
	kind{kind},
	internalFormat{internalFormat},
	width{0}, height{0} {
	glGenTextures(1, &id);
}

TextureBuffer::~TextureBuffer() {
	glDeleteTextures(1, &id);
}

void TextureBuffer::bind(const unsigned int &unit) const {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(kind, id);
}

void TextureBuffer::setSize(int width, int height) {
	bind();
	glTexImage2D(kind, 0, internalFormat, width, height, 0, RGB, UNSIGNED_BYTE, nullptr);

	this->width = width;
	this->height = height;
}

void TextureBuffer::setData(const void *data, Format format, Type type) {
	bind();
	glTexSubImage2D(kind, 0, 0, 0, width, height, format, type, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

TextureBuffer::Format TextureBuffer::getInternalFormat() {
	return (Format) internalFormat;
}

void TextureBuffer::generateMipmap() {
	bind();
	glGenerateMipmap(kind);
}


//////////////////////////////////////////////////////////////////////////////////////

Texture::Texture(Format internalFormat): TextureBuffer{GL_TEXTURE_2D, internalFormat} {}
