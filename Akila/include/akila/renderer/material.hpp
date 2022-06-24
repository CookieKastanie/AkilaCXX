#pragma once

#include "akila/memory/ref.hpp"
#include "akila/renderer/shader.hpp"

namespace akila {
	class Material {
	public:
		Material() = default;
		Material(std::string const &shader);

		void render() {
			shader.bind();
			//glBindBuffer(GL_ARRAY_BUFFER, 0);
			//glDrawArrays(GL_TRIANGLES, 0, 3);
		}

	private:
		Shader shader;
	};

	class MaterialInstance {
	public:
		MaterialInstance(Ref<Material> material);

	private:
		Ref<Material> material;
	};
}
