#pragma once

#include "Akila/graphics/ShaderBuilder.hpp"
#include <memory>

namespace Akila {
	class Material {
		private:
			std::shared_ptr<Shader> shader;

		public:
			Material();
		
			Shader *getShader();
	};
}
