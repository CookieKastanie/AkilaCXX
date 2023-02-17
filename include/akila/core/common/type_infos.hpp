#pragma once

#include <string>

namespace akila {
	using TypeId = std::size_t;

	template<typename T>
	TypeId getTypeId() {
		return typeid(T).hash_code();
	}

	template<typename T>
	std::string getTypeName() {
		return typeid(T).name();
	}
}
