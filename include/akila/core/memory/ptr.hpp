#pragma once

#include <memory>

namespace akila {
	template<typename T>
	using Ptr = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ptr<T> createPtr(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}
