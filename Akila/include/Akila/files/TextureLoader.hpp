#pragma once

#include "Akila/graphics/gl/Texture.hpp"
#include <functional>
#include <string>

namespace Akila {
	class TextureLoader {
	public:
		static void color(Texture *texture, std::string const &path, std::function<void()> const &callback);

		static TextureBuffer::Format stringToFormat(std::string const &str);
		static TextureBuffer::WrapMode stringToWrapMode(std::string const &str);
		static TextureBuffer::FilterMode stringToFilterMode(std::string const &str);
	};
};
