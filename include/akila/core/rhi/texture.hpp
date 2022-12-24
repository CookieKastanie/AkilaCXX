#pragma once

#include <glad/glad.h>
#include "akila/core/math/math.hpp"
#include "akila/core/memory/ptr.hpp"

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
			RGBA16F = GL_RGBA16F,

			RGB32F = GL_RGB32F,
			RGBA32F = GL_RGBA32F
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
		virtual void setSize(IVec2 const &size) = 0;
		virtual void setData(void const *data, Format format, Type type, unsigned int mipLevel = 0) = 0;

		virtual Ptr<std::uint8_t> getData(unsigned int mip = 0) const;

		IVec2 const &getSize() const;

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
			Parameters(
				FilterMode minFilter = FilterMode::LINEAR,
				FilterMode magFilter = FilterMode::LINEAR,
				WrapMode wrapS = WrapMode::REPEAT,
				WrapMode wrapT = WrapMode::REPEAT,
				WrapMode wrapR = WrapMode::REPEAT,
				Vec4 const &borderColor = {0, 0, 0, 1}
			);

			FilterMode minFilter;
			FilterMode magFilter;

			WrapMode wrapS;
			WrapMode wrapT;
			WrapMode wrapR;

			Vec4 borderColor;
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

		void setSize(IVec2 const &size) override;
		void setData(void const *data, Format format, Type type, unsigned int mipLevel = 0) override;
	};

	/////

	class TextureCubmap: public TextureBuffer {
	public:
		TextureCubmap(Format format = Format::RGB);

		void setSize(IVec2 const &size) override;
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

	/////

	class Texture2DMultisample: public TextureBuffer {
	public:
		Texture2DMultisample(int samples = 4, Format format = Format::RGB);

		Texture2DMultisample(Texture2DMultisample &&other) noexcept;
		Texture2DMultisample &operator=(Texture2DMultisample &&other) noexcept;

		void setSize(IVec2 const &size) override;
		void setData(void const *data, Format format, Type type, unsigned int mipLevel = 0) override;

		int getSampleCount();

	private:
		GLsizei samples;
	};

	/////

	class Texture3D: public TextureBuffer {
	public:
		Texture3D(Format format = Format::RGB);

		Texture3D(Texture3D &&other) noexcept;
		Texture3D &operator=(Texture3D &&other) noexcept;

		void setSize(IVec2 const &size) override;
		void setSize(IVec3 const &size);

		int getDepth() const;

		void setData(void const *data, Format format, Type type, unsigned int mipLevel = 0) override;

	private:
		GLsizei depth;
	};
}
