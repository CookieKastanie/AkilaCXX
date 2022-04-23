#pragma once

#include "Akila/graphics/gl/Shader.hpp"
#include <string>

namespace Akila {
	class ShaderLoader {
	public:
		static Shader *create(std::string const &path);
	};
}
