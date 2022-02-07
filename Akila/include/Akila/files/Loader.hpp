#pragma once

#include "Akila/graphics/gl/Texture.hpp"
#include "Akila/graphics/gl/Shader.hpp"
#include "Akila/graphics/Mesh.hpp"

namespace Akila {
	class Loader {
	public:
		//static void splitString(std::vector<std::string> &list, std::string &str, const std::string &delimiter);
		//static void trimString(std::string &str);

		static std::shared_ptr<Shader> shader(const std::string &path);
	};
}
