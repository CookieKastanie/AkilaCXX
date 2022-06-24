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
			glBindVertexArray(vb);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0);
		}

	private:
		Shader shader;

		GLuint vb;
	};

	class MaterialInstance {
	public:
		MaterialInstance(Ref<Material> material);

	private:
		Ref<Material> material;
	};
}
