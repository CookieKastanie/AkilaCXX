#pragma once

#include "akila/core/memory/ref.hpp"
#include "akila/core/rhi/texture.hpp"
#include "akila/core/math/math.hpp"

namespace akila {
	class FrameBuffer {
	public:
		FrameBuffer();
		~FrameBuffer();

		FrameBuffer(FrameBuffer &&other) noexcept;
		FrameBuffer &operator=(FrameBuffer &&other) noexcept;

		FrameBuffer(FrameBuffer const &other) = delete;
		FrameBuffer &operator=(FrameBuffer const &other) = delete;

		enum class Attachement: GLenum {
			TEXTURE = GL_TEXTURE_2D,
			MULTISAMPLED_TEXTURE = GL_TEXTURE_2D_MULTISAMPLE,
			CUBE_MAP_POSITIVE_X = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			CUBE_MAP_NEGATIVE_X = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			CUBE_MAP_POSITIVE_Y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			CUBE_MAP_NEGATIVE_Y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			CUBE_MAP_POSITIVE_Z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			CUBE_MAP_NEGATIVE_Z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		};

		static Attachement nextAttachement(Attachement attachement);
		
		void setTexture(Ref<TextureBuffer> const &texture, int unit = 0, Attachement attachment = Attachement::TEXTURE);
		void setTextureWeak(TextureBuffer const *texture, int unit = 0, Attachement attachment = Attachement::TEXTURE, unsigned int mip = 0);
		Ref<TextureBuffer> &getTexture(int unit);

		void setDepthTexture(Ref<TextureBuffer> const &texture, Attachement attachment = Attachement::TEXTURE);
		Ref<TextureBuffer> &getDepthTexture();

		void changeAttachment(int unit, Attachement attachment, unsigned int mipLevel = 0);

		void prepare();

		void bind();
		void bind(int unit);
		void bind(IVec2 size);
		void unbind();

		void setViewport(int unit);
		void setViewport(IVec2 size);

		void blitToDefault(int unit = 0, TextureBuffer::FilterMode filter = TextureBuffer::FilterMode::NEAREST);
		void blitTo(int seflUnit, FrameBuffer *fb, TextureBuffer::FilterMode filter = TextureBuffer::FilterMode::NEAREST);

		void resizeAll(IVec2 size);

	private:
		static int const MAX_ATTACHMENT_COUNT = 8;

		GLuint id;
		Ref<TextureBuffer> textures[MAX_ATTACHMENT_COUNT];
		Ref<TextureBuffer> depthTexture;
	};
}
