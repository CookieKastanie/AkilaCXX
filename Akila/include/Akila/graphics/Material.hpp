#pragma once

#include "Akila/graphics/ShaderBuilder.hpp"
#include <memory>

namespace Akila {
	class Material {
		private:
			std::string name;
			std::shared_ptr<Shader> shader;

		public:
			Material();
		
			void loadFromFile(const std::string &fileName);
			Shader *getShader();
			std::string &getName();
	};
}
