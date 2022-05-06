#pragma once

#include <cstdint>

namespace akila {
	using TypeName = std::size_t;

	template<typename T>
	inline TypeName &&getTypeName() {
		return typeid(T).hash_code();
	}
}
