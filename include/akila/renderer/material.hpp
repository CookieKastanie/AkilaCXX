#pragma once

#include "akila/memory/ref.hpp"
#include "akila/memory/ptr.hpp"
#include "akila/renderer/shader.hpp"
#include "akila/math/math.hpp"
#include "akila/renderer/texture.hpp"

namespace akila {
	class Material {
	public:
		Material() = default;
		explicit Material(Ref<Shader> shader);
		explicit Material(Ref<Material> material);

		//deplacement
		Material(Material &&other) noexcept;
		Material &operator=(Material &&other) noexcept;

		// copie
		Material(Material const &other);
		Material &operator=(Material const &other);

		Ref<Shader> getShaderRef() const;
		Shader *getShader() const;
		Ptr<Material> copy();

		/* Indique quel uniform le materiau utilisera */
		bool use(std::string const &name);

		void writeRaw(UniformInfos const *infos, void const *data, std::size_t byteCount);

		void write(std::string const &name, int data);
		void write(std::string const &name, float data);

		void write(std::string const &name, Vec2 const &data);
		void write(std::string const &name, std::vector<Vec2> const &data);

		void write(std::string const &name, Vec3 const &data);
		void write(std::string const &name, std::vector<Vec3> const &data);

		void write(std::string const &name, Vec4 const &data);
		void write(std::string const &name, std::vector<Vec4> const &data);

		void write(std::string const &name, Mat4 const &data);
		void write(std::string const &name, std::vector<Mat4> const &data);

		void affect(Ref<TextureBuffer> const &texRef, int unit);
		void affect(Ref<TextureBuffer> const &texRef, std::string const &name);

		void send();

	private:
		Ref<Shader> shader;

		std::vector<UniformInfos const *> usedUniforms;
		std::unordered_map<std::string, UniformInfos const *> uniforms;

		std::vector<std::uint8_t> uniformData;

		struct TextureBinding {
			int unit;
			Ref<TextureBuffer> textureBuffer;
		};
		std::vector<TextureBinding> textures;
	};
}
