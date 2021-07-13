#pragma once

#include <glad/glad.h>
#include "Akila/files/FileSystem.hpp"

namespace Akila {
	class TextureBuffer {
	protected:
		GLuint id;
		GLenum kind;
		GLenum internalFormat;

		int width;
		int height;

	public:
		enum Format: GLenum {
			DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
			DEPTH_STENCIL = GL_DEPTH_STENCIL,
			RED = GL_RED,
			RG = GL_RG,
			RGB = GL_RGB,
			RGBA = GL_RGBA,

			SRGB = GL_SRGB,
			SRGB_ALPHA = GL_SRGB_ALPHA,

			RGB16F = GL_RGB16F,
			RGBA16F = GL_RGBA16F
		};

		enum Type: GLenum {
			UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
			INT = GL_INT,
			FLOAT = GL_FLOAT
		};

		TextureBuffer(unsigned int kind, Format internalFormat);
		virtual ~TextureBuffer();

		void bind(const unsigned int &unit = 0) const;
		virtual void setSize(int width, int height);
		virtual void setData(const void *data, Format format, Type type);

		Format getInternalFormat();

		virtual void generateMipmap();
	};


	class Texture: public TextureBuffer {
	public:
		Texture(Format internalFormat = RGBA);
	};
}
