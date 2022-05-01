#pragma once

#include "akila/ecs/entityId.hpp"
#include "akila/ecs/signature.hpp"

#include <array>
#include <stack>
#include <set>
#include <iostream>

namespace akila {
	class EntityManager {
	private:
		friend class Coordinator;

		std::stack<EntityId> availableEntityIds;
		std::set<EntityId> createdEntityIds;
		std::array<Signature, MAX_ENTITY_COUNT> entitySignatures;
		//std::array<bool, MAX_ENTITY_COUNT> entityIsAlive; ???

		EntityManager() {
			for(long i = MAX_ENTITY_COUNT - 1; i >= 0; --i) {
				availableEntityIds.push(static_cast<EntityId>(i));
			}
		}

		EntityId create() {
			if(availableEntityIds.empty()) {
				std::cerr << "No more entity available !" << std::endl;
				return MAX_ENTITY_COUNT - 1;
			}

			EntityId id = availableEntityIds.top();
			availableEntityIds.pop();
			entitySignatures[id] = 0;
			createdEntityIds.insert(id);
			return id;
		}

		void erase(EntityId id) {
			createdEntityIds.erase(id);
			availableEntityIds.push(id);
		}

		Signature &getSignature(EntityId id) {
			return entitySignatures[id];
		}

		void setSignature(EntityId id, Signature const &signature) {
			entitySignatures[id] = signature;
		}
	};
}
