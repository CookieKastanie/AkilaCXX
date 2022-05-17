#pragma once

#include <functional>

namespace akila {
	template<typename T>
	using SignalCallback = std::function<void(T const &)>;
}
