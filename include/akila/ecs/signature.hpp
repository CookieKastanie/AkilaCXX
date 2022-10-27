#pragma once

#include <bitset>
#include "akila/ecs/component_manager.hpp"

namespace akila {
	using Signature = std::bitset<internal::MAX_COMPONENT_TYPES>;
}
