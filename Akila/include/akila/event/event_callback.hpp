#pragma once

#include <functional>

namespace akila {
	template<typename T>
	using EventCallback = std::function<void(T const &)>;
}
