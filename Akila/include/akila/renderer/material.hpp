#pragma once

#include "akila/memory/ref.hpp"
#include "akila/renderer/shader.hpp"

namespace akila {
	class MaterialInstance {
	public:
		MaterialInstance(Ref<Material> material);

	private:
		Ref<Material> material;
	};

	class Material {
	public:
		Material(std::string const &shader);

	private:
		Shader shader;
	};
}
