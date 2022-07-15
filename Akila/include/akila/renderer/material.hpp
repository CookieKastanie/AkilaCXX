#pragma once

#include "akila/memory/ref.hpp"
#include "akila/renderer/shader.hpp"
#include "akila/math/math.hpp"

namespace akila {
	class Material {
	public:
		Material() = default;
		Material(Ref<Shader> shader);
		Material(std::string const &shaderTxt);

		Ref<Shader> getShaderRef() const;

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

		void send();

	private:
		Ref<Shader> shader;

		std::vector<UniformInfos const *> usedUniforms;
		std::unordered_map<std::string, UniformInfos const *> uniforms;

		std::vector<std::uint8_t> uniformData;
	};
}
