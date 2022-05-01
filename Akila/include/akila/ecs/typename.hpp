#pragma once

#include <cstdint>

#define GET_TYPE_NAME(T) typeid(T).hash_code();

namespace akila {
	using TypeName = std::size_t;
}
