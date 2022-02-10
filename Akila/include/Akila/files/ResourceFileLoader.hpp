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

	public:
		static void fillResourcePool(ResourcePool *rp, std::string const &path, std::function<void()> const &callback);
	};
}
