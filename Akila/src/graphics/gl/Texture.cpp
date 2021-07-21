#include "Akila/graphics/gl/Texture.hpp"
#include <iostream>

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

unsigned int TextureBuffer::getId() const {
	return id;
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

void TextureBuffer::setData(const void *data, Format format, Type type, unsigned int mipLevel) {
	bind();
	glTexSubImage2D(kind, mipLevel, 0, 0, width, height, format, type, data);
}

int TextureBuffer::getWidth() const {
	return width;
}

int TextureBuffer::getHeight() const {
	return height;
}

TextureBuffer::Parameters::Parameters(): wrapS{REPEAT}, wrapT{REPEAT}, wrapR{REPEAT}, minFilter{LINEAR}, magFilter{LINEAR} {}

void TextureBuffer::setParameters(const Parameters &params) {
	bind();

	glTexParameteri(kind, GL_TEXTURE_WRAP_S, params.wrapS);
	glTexParameteri(kind, GL_TEXTURE_WRAP_T, params.wrapT);
	glTexParameteri(kind, GL_TEXTURE_WRAP_R, params.wrapR);
	glTexParameteri(kind, GL_TEXTURE_MIN_FILTER, params.minFilter);
	glTexParameteri(kind, GL_TEXTURE_MAG_FILTER, params.magFilter);
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

//////////////////////////////////////////////////////////////////////////////////////

DepthTexture::DepthTexture(Format internalFormat): TextureBuffer{GL_TEXTURE_2D, internalFormat} {}

//////////////////////////////////////////////////////////////////////////////////////

CubeMapTexture::CubeMapTexture(Format internalFormat): TextureBuffer{GL_TEXTURE_CUBE_MAP, internalFormat} {}

void CubeMapTexture::setSize(int width, int height) {
	bind();
	for(unsigned int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, RGB, UNSIGNED_BYTE, nullptr);
	}

	this->width = width;
	this->height = height;
}

void CubeMapTexture::setData(const void *data, Format format, Type type, unsigned int mipLevel) {
	std::cerr << "CubeMapTexture -> setData : You need to specify a face" << std::endl;
}

void CubeMapTexture::setData(Face face, const void *data, Format format, Type type, unsigned int mipLevel) {
	bind();
	glTexSubImage2D(face, mipLevel, 0, 0, width, height, format, type, data);
}
