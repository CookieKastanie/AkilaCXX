#pragma once

#include "glad/glad.h"
#include "Akila/core/Memory.hpp"
#include "Akila/graphics/gl/Texture.hpp"
#include "Akila/core/Display.hpp"

namespace Akila {
	class FrameBuffer {
	private:
		static int const MAX_ATTACHMENT_COUNT = 8;

		GLuint id;
		Ref<TextureBuffer> textures[MAX_ATTACHMENT_COUNT];
		Ref<TextureBuffer> depthTexture;

	public:
		FrameBuffer();
		~FrameBuffer();

		void setTexture(Ref<Texture> const &texture, int unit = 0);
		void setTexture(Ref<CubeMapTexture> const &texture, int unit = 0);
		Ref<TextureBuffer> &getTexture(int unit);

		void setDepthTexture(Ref<TextureBuffer> const &texture);

		Ref<TextureBuffer> &getDepthTexture();

		void changeAttachment(int unit, GLenum attachment, unsigned int mipLevel = 0);

		void prepare();

		void bind(int unit = 0);
		void bindWithSize(int width, int height);
		void unbind();

		void blitToDisplay(int unit, Display *display, TextureBuffer::FilterMode filter = TextureBuffer::FilterMode::NEAREST);
		void blitTo(int seflUnit, FrameBuffer *fb, TextureBuffer::FilterMode filter = TextureBuffer::FilterMode::NEAREST);

		void resizeAll(int width, int height);
	};
}
