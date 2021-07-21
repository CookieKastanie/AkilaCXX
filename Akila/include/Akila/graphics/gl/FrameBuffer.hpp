#pragma once

#include "glad/glad.h"
#include <memory>
#include "Akila/graphics/gl/Texture.hpp"

namespace Akila {
	class FrameBuffer {
	private:
		static int const MAX_ATTACHMENT_COUNT = 6;

		GLuint id;
		std::shared_ptr<TextureBuffer> textures[MAX_ATTACHMENT_COUNT];
		std::shared_ptr<TextureBuffer> depthTexture;

	public:
		FrameBuffer();
		~FrameBuffer();

		void setTexture(const std::shared_ptr<Texture> &texture, int unit = 0);
		void setTexture(const std::shared_ptr<CubeMapTexture> &texture, int unit = 0);
		std::shared_ptr<TextureBuffer> getTexture(int unit);

		void setDepthTexture(const std::shared_ptr<DepthTexture> &texture);

		std::shared_ptr<TextureBuffer> getDepthTexture();

		void changeAttachment(int unit, GLenum attachment, unsigned int mipLevel = 0);

		void prepare();

		void bind(int unit = 0);
		void bindWithSize(int width, int height);
		void unbind();

		void blitToScreen(int unit = 0, TextureBuffer::FilterMode filter = TextureBuffer::FilterMode::NEAREST);
		void blitTo(int seflUnit, FrameBuffer *fb, TextureBuffer::FilterMode filter = TextureBuffer::FilterMode::NEAREST);

		void resizeAll(int width, int height);
	};
}
