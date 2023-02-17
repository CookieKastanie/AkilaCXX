#pragma once

#include <cstdint>

namespace akila {
	namespace internal {
		using EntityId = std::uint16_t;
		const EntityId MAX_ENTITY_COUNT = 7777;
		const EntityId INVALID_ENTITY_ID = static_cast<EntityId>(-1);
	}
}
