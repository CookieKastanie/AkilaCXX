#include "akila/core/renderer/texture.hpp"

using namespace akila;

TextureBuffer::TextureBuffer(Kind kind, Format internalFormat):
	kind{static_cast<GLenum>(kind)},
	internalFormat{static_cast<GLenum>(internalFormat)},
	size{0, 0} {

	glGenTextures(1, &id);
}

TextureBuffer::~TextureBuffer() {
	glDeleteTextures(1, &id);
}

TextureBuffer::TextureBuffer(TextureBuffer &&other) noexcept:
	id{other.id},
	kind{other.kind},
	internalFormat{other.internalFormat},
	size{other.size} {

	other.id = 0;
}

TextureBuffer &TextureBuffer::operator=(TextureBuffer &&other) noexcept {
	id = other.id;
	kind = other.kind;
	internalFormat = other.internalFormat;
	size = other.size;

	other.id = 0;

	return *this;
}

unsigned int TextureBuffer::getId() const {
	return id;
}

void TextureBuffer::bind(unsigned int unit) const {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(kind, id);
}

IVec2 const &TextureBuffer::getSize() const {
	return size;
}

Ptr<std::uint8_t> TextureBuffer::getData(unsigned int mip) const {
	std::size_t byteSize = size.x * size.y * 4 * sizeof(GLuint);
	Ptr<std::uint8_t> buffer = Ptr<std::uint8_t>(new std::uint8_t[byteSize]);

	glBindTexture(kind, id);
	glGetTexImage(kind, mip, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());

	return buffer;
}

TextureBuffer::Parameters::Parameters(FilterMode minFilter, FilterMode magFilter, WrapMode wrapS, WrapMode wrapT, WrapMode wrapR, Vec4 const &borderColor):
	minFilter{minFilter},
	magFilter{magFilter},
	wrapS{wrapS},
	wrapT{wrapT},
	wrapR{wrapR},
	borderColor{borderColor} {}

void TextureBuffer::setParameters(Parameters const &params) {
	bind();

	glTexParameteri(kind, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(params.minFilter));
	glTexParameteri(kind, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(params.magFilter));

	glTexParameteri(kind, GL_TEXTURE_WRAP_S, static_cast<GLint>(params.wrapS));
	glTexParameteri(kind, GL_TEXTURE_WRAP_T, static_cast<GLint>(params.wrapT));
	glTexParameteri(kind, GL_TEXTURE_WRAP_R, static_cast<GLint>(params.wrapR));

	if(
		params.wrapS == TextureBuffer::WrapMode::CLAMP_TO_BORDER ||
		params.wrapT == TextureBuffer::WrapMode::CLAMP_TO_BORDER ||
		params.wrapR == TextureBuffer::WrapMode::CLAMP_TO_BORDER) {

		glTexParameterfv(kind, GL_TEXTURE_BORDER_COLOR, &params.borderColor[0]);
	}
}

TextureBuffer::Format TextureBuffer::getInternalFormat() {
	return (Format)internalFormat;
}

void TextureBuffer::generateMipmap() {
	bind();
	glGenerateMipmap(kind);
}

int TextureBuffer::calculatMipmapCount() {
	return 1 + static_cast<int>(floor(log2(max(size.x, size.y))));
}

/////

Texture2D::Texture2D(Format format): TextureBuffer{Kind::TEXTURE_2D, format} {
	setParameters({});
}

void Texture2D::setSize(IVec2 const &size) {
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

/////

TextureCubmap::TextureCubmap(Format format): TextureBuffer{Kind::TEXTURE_CUBE_MAP, format} {
	setParameters({});
}

void TextureCubmap::setSize(IVec2 const &size) {
	bind();
	for(unsigned int i = 0; i < 6; ++i) {
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat,
			size.x, size.y, 0, static_cast<GLenum>(Format::RGB), static_cast<GLenum>(Type::UNSIGNED_BYTE), nullptr
		);
	}

	this->size = size;
}

void TextureCubmap::setData(void const *data, Format format, Type type, unsigned int mipLevel) {
	setData(data, Face::FRONT, format, type, mipLevel);
}

void TextureCubmap::setData(const void *data, Face face, Format format, Type type, unsigned int mipLevel) {
	bind();
	glTexSubImage2D(
		static_cast<GLenum>(face), mipLevel, 0, 0, size.x, size.y,
		static_cast<GLenum>(format), static_cast<GLenum>(type), data
	);
}

/////

Texture2DMultisample::Texture2DMultisample(int samples, Format format):
	TextureBuffer{Kind::TEXTURE_2D_MULTISAMPLE, format},
	samples{samples} {

}

Texture2DMultisample::Texture2DMultisample(Texture2DMultisample &&other) noexcept:
	TextureBuffer{static_cast<Kind>(other.kind), static_cast<Format>(other.internalFormat)},
	samples{other.samples} {

}

Texture2DMultisample &Texture2DMultisample::operator=(Texture2DMultisample &&other) noexcept {
	TextureBuffer::operator=(std::move(other));
	samples = other.samples;

	return *this;
}

void Texture2DMultisample::setSize(IVec2 const &size) {
	bind();
	glTexImage2DMultisample(kind, samples, internalFormat, size.x, size.y, GL_TRUE);
	this->size = size;
}

void Texture2DMultisample::setData(void const *data, Format format, Type type, unsigned int mipLevel) {
	std::cerr << "Can't set data to multisampled texture" << std::endl;
}

int Texture2DMultisample::getSampleCount() {
	return samples;
}

/////

Texture3D::Texture3D(Format format): TextureBuffer{Kind::TEXTURE_3D, format}, depth{1} {
	setParameters({});
}

Texture3D::Texture3D(Texture3D &&other) noexcept:
	TextureBuffer{static_cast<Kind>(other.kind), static_cast<Format>(other.internalFormat)},
	depth{other.depth} {

}

Texture3D &Texture3D::operator=(Texture3D &&other) noexcept {
	TextureBuffer::operator=(std::move(other));
	depth = other.depth;

	return *this;
}

void Texture3D::setSize(IVec2 const &size) {
	setSize({size.x, size.y, 1});
}

void Texture3D::setSize(IVec3 const &size) {
	bind();

	glTexImage3D(
		kind, 0, internalFormat, size.x, size.y, size.y, 0,
		internalFormat == GL_DEPTH_COMPONENT ?
		GL_DEPTH_COMPONENT : static_cast<GLenum>(Format::RGB),
		static_cast<GLenum>(Type::UNSIGNED_BYTE), nullptr
	);

	this->size = size;
	this->depth = size.z;
}

int Texture3D::getDepth() const {
	return depth;
}

void Texture3D::setData(void const *data, Format format, Type type, unsigned int mipLevel) {
	bind();

	glTexSubImage3D(
		kind, mipLevel, 0, 0, 0, size.x, size.y, depth,
		static_cast<GLenum>(format), static_cast<GLenum>(type), data
	);
}
