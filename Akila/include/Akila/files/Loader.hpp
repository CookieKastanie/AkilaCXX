#pragma once

#include "Akila/core/Task.hpp"
#include "Akila/graphics/gl/Texture.hpp"
#include "Akila/graphics/gl/Shader.hpp"

namespace Akila {
	class Loader {
	public:
		static void splitString(std::vector<std::string> &list, std::string &str, const std::string &delimiter);
		static void trimString(std::string &str);

		static void asyncTexture(Texture *texture, const std::string &path, const bool generateMips, TaskManager *tm = nullptr);
		static std::shared_ptr<Shader> shader(const std::string &path);
	};
}
