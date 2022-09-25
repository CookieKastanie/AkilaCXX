#pragma once

#include "akila/memory/ref.hpp"
#include "akila/renderer/texture.hpp"
#include "akila/math/math.hpp"

namespace akila {
	class FrameBuffer {
	public:
		FrameBuffer();
		~FrameBuffer();

		enum class Attachement {
			TEXTURE = GL_TEXTURE_2D,
			CUBE_MAP_POSITIVE_X = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			CUBE_MAP_NEGATIVE_X = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			CUBE_MAP_POSITIVE_Y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			CUBE_MAP_NEGATIVE_Y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			CUBE_MAP_POSITIVE_Z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			CUBE_MAP_NEGATIVE_Z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		};

		void setTexture(Ref<TextureBuffer> const &texture, int unit = 0, Attachement attachment = Attachement::TEXTURE);
		Ref<TextureBuffer> &getTexture(int unit);

		void setDepthTexture(Ref<TextureBuffer> const &texture);
		Ref<TextureBuffer> &getDepthTexture();

		void changeAttachment(int unit, Attachement attachment, unsigned int mipLevel = 0);

		void prepare();

		void bind(int unit = 0);
		void bindWithSize(IVec2 size);
		void unbind();

		void blitToDefault(int unit, TextureBuffer::FilterMode filter = TextureBuffer::FilterMode::NEAREST);
		void blitTo(int seflUnit, FrameBuffer *fb, TextureBuffer::FilterMode filter = TextureBuffer::FilterMode::NEAREST);

		void resizeAll(IVec2 size);

	private:
		static int const MAX_ATTACHMENT_COUNT = 8;

		GLuint id;
		Ref<TextureBuffer> textures[MAX_ATTACHMENT_COUNT];
		Ref<TextureBuffer> depthTexture;
	};
}
