#pragma once

#include <cstdint>

namespace akila::internal {
	using EntityId = std::uint16_t;
	const EntityId MAX_ENTITY_COUNT = 7777;
	const EntityId INVALID_ENTITY_ID = -1;
}