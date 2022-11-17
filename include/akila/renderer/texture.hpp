#pragma once

#include <glad/glad.h>
#include "akila/math/math.hpp"
#include "akila/memory/ptr.hpp"

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

		TextureBuffer(TextureBuffer &&other) noexcept;
		TextureBuffer &operator=(TextureBuffer &&other) noexcept;

		TextureBuffer(TextureBuffer const &other) = delete;
		TextureBuffer &operator=(TextureBuffer const &other) = delete;

		unsigned int getId() const;

		void bind(unsigned int unit = 0) const;
		virtual void setSize(IVec2 size) = 0;
		virtual void setData(void const *data, Format format, Type type, unsigned int mipLevel = 0) = 0;

		virtual Ptr<std::uint8_t> getData(unsigned int mip = 0) const;

		IVec2 getSize() const;

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
		virtual int calculatMipmapCount();

	protected:
		GLuint id;
		GLenum kind;
		GLenum internalFormat;

		IVec2 size;
	};

	/////

	class Texture2D: public TextureBuffer {
	public:
		Texture2D(Format format = Format::RGB);

		void setSize(IVec2 size) override;
		void setData(void const *data, Format format, Type type, unsigned int mipLevel = 0) override;
	};

	/////

	class TextureCubmap: public TextureBuffer {
	public:
		TextureCubmap(Format format = Format::RGB);

		void setSize(IVec2 size) override;
		void setData(void const *data, Format format, Type type, unsigned int mipLevel = 0) override;

		enum class Face: GLenum {
			FRONT = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
			BACK = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			LEFT = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			RIGHT = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			BOTTOM = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			TOP = GL_TEXTURE_CUBE_MAP_POSITIVE_Y
		};

		void setData(const void *data, Face face, Format format, Type type, unsigned int mipLevel = 0);
	};

	/* TODO
	class Texture3D: public TextureBuffer {

	};

	class Texture2DMultisample: public TextureBuffer {

	};

	class Texture2DArray: public TextureBuffer {

	};
	*/
}
