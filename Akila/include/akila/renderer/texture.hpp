#pragma once

#include <glad/glad.h>

namespace akila {
	class TextureBuffer {
	public:
		enum class Kind: GLenum {
			TEXTURE_1D = GL_TEXTURE_1D,
			TEXTURE_2D = GL_TEXTURE_2D,
			TEXTURE_3D = GL_TEXTURE_3D,
			TEXTURE_1D_ARRAY = GL_TEXTURE_1D_ARRAY,
			TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
			TEXTURE_RECTANGLE = GL_TEXTURE_RECTANGLE,
			TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP,
			TEXTURE_CUBE_MAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY,
			TEXTURE_BUFFER = GL_TEXTURE_BUFFER,
			TEXTURE_2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE,
			TEXTURE_2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
		};

		enum class Format: GLenum {
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

		enum class Type: GLenum {
			UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
			INT = GL_INT,
			FLOAT = GL_FLOAT
		};

		TextureBuffer(Kind type, Format internalFormat);
		virtual ~TextureBuffer();

		unsigned int getId() const;

		void bind(unsigned int unit = 0) const;
		virtual void setSize(int width, int height) = 0;
		virtual void setData(void const *data, Format format, Type type, unsigned int mipLevel = 0) = 0;

		int getWidth() const;
		int getHeight() const;

		enum class WrapMode: GLint {
			CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
			CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
			MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
			REPEAT = GL_REPEAT
		};
		enum class FilterMode: GLint {
			NEAREST = GL_NEAREST,
			LINEAR = GL_LINEAR,
			NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
			LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
			NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
			LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
		};
		struct Parameters {
			Parameters();

			WrapMode wrapS;
			WrapMode wrapT;
			WrapMode wrapR;

			FilterMode minFilter;
			FilterMode magFilter;
		};
		void setParameters(Parameters const &params);

		Format getInternalFormat();

		virtual void generateMipmap();

	protected:
		GLuint id;
		GLenum kind;
		GLenum internalFormat;

		int width;
		int height;
	};

	/////

	class Texture2D: public TextureBuffer {
	public:
		Texture2D(Format format = Format::RGB);

		void setSize(int width, int height) override;
		void setData(void const *data, Format format, Type type, unsigned int mipLevel = 0) override;
	};

	/* TODO
	class Texture3D: public TextureBuffer {

	};

	class Texture2DMultisample: public TextureBuffer {

	};

	class Texture2DArray: public TextureBuffer {

	};

	class TextureCubmap: public TextureBuffer {

	};
	*/
}
