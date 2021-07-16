#pragma once

#include "Akila/graphics/gl/Shader.hpp"
#include "Akila/graphics/gl/Texture.hpp"
#include <memory>
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
			std::shared_ptr<TextureBuffer> textureBuffer;
		};

	private:
		static int lastId;

		int id;

		std::shared_ptr<Shader> shader;

		std::vector<UniformValue> uniformsFloat;
		std::vector<UniformValue> uniformsInts;

		std::vector<TextureBinding> textures;

	public:
		static int currentUsed;

		Material();

		int getId() const;

		Shader *getShader() const;
		void setShader(const std::shared_ptr<Shader> &shader);

		void addUniformValue(const UniformValue &uv, bool isInts = false);
		void sendUniforms() const;

		void addTextureBinding(const TextureBinding &tb);
		void bindTextures() const;
	};
}
