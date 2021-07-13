#pragma once

#include "Akila/graphics/gl/Shader.hpp"
#include <memory>

namespace Akila {
	class Material {
	private:
		std::shared_ptr<Shader> shader;

	public:
		Material();

		Shader *getShader();
		void setShader(const std::shared_ptr<Shader> &shader);
	};
}
