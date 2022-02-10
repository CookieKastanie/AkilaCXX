#pragma once

#include "Akila/graphics/gl/Shader.hpp"
#include "Akila/graphics/gl/Texture.hpp"
#include "Akila/core/ResourceReference.hpp"
#include <vector>

namespace Akila {
	class Material {
	public:
		union UniformValueType {
			float f;
			int i;
		};
		struct UniformValue {
			unsigned int uid;
			std::vector<UniformValueType> values;
		};

		struct TextureBinding {
			unsigned int unit;
			ResourceReference<TextureBuffer> textureBuffer;
		};

	private:
		static int lastId;

		int id;

		ResourceReference<Shader> shader;

		std::vector<UniformValue> uniformsFloat;
		std::vector<UniformValue> uniformsInts;

		std::vector<TextureBinding> textures;

	public:
		static int currentUsed;

		Material();

		int getId() const;

		Shader *getShader() const;
		void setShader(ResourceReference<Shader> const &shader);

		void addUniformValue(const UniformValue &uv, bool isInts = false);
		void sendUniforms() const;

		void addTextureBinding(TextureBinding const &tb);
		void bindTextures() const;
	};
}
