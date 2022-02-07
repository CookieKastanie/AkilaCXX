#pragma once

#include "Akila/graphics/gl/Texture.hpp"
#include <functional>
#include <string>

namespace Akila {
	class TextureLoader {
		static void color(Texture *texture, std::string const &path, std::function<void()> const &callback);
	};
};
