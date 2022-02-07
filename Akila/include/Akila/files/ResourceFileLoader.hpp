#pragma once

#include "Akila/core/ResourcePool.hpp"
#include <string>
#include <functional>

namespace Akila {
	class ResourceFileLoader {
	private:
		static TextureBuffer::Format stringToFormat(std::string const &str);
		static TextureBuffer::WrapMode stringToWrapMode(std::string const &str);
		static TextureBuffer::FilterMode stringToFilterMode(std::string const &str);

	public:
		static void fillResourcePool(ResourcePool *rp, std::string const &path, std::function<void()> callback);
	};
}
