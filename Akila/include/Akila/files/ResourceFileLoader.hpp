#pragma once

#include "Akila/core/ResourcePool.hpp"
#include <string>
#include <functional>

namespace Akila {
	class ResourceFileLoader {
	private:
		static int count;
		static std::function<void()> countCB; 
		static std::function<void()> callback;

		static TextureBuffer::Format stringToFormat(std::string const &str);
		static TextureBuffer::WrapMode stringToWrapMode(std::string const &str);
		static TextureBuffer::FilterMode stringToFilterMode(std::string const &str);

	public:
		static void fillResourcePool(ResourcePool *rp, std::string const &path, std::function<void()> const &callback);
	};
}
