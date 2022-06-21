#pragma once

#include <set>
#include "akila/ecs/signature.hpp"
#include "akila/ecs/entity.hpp"

namespace akila {
	namespace internal {
		class SystemManager;
		class Coordinator;
	}

	class System {
	public:
		System(Signature signature): signature{signature} {};
		virtual ~System() = default;

	protected:
		std::set<Entity> entities;

		virtual void onAdd(Entity entity) {};
		virtual void onRemove(Entity entity) {};

	private:
		friend class internal::SystemManager;
		friend class internal::Coordinator;

		Signature signature;

		bool isCompatible(Signature const &s) {
			return (signature & s) == signature;
		}

		void eraseEntity(internal::EntityId entityId) {
			if(entities.erase(entityId))
				onRemove(entityId);
		}

		void addIfCompatible(internal::EntityId entityId, Signature const &s) {
			if(isCompatible(s)) {
				if(entities.insert(entityId).second)
					onAdd(entityId);
			}
		}

		void eraseIfNotCompatible(internal::EntityId entityId, Signature const &s) {
			if(!isCompatible(s)) {
				eraseEntity(entityId);
			}
		}
	};
}
