#pragma once

#include <string>

namespace akila {
	using TypeId = std::size_t;

	template<typename T>
	inline TypeId getTypeId() {
		return typeid(T).hash_code();
	}

	template<typename T>
	inline std::string getTypeName() {
		return typeid(T).name();
	}
}
