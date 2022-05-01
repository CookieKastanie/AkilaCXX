#pragma once

#include <set>
#include "akila/ecs/signature.hpp"
#include "akila/ecs/entity.hpp"

namespace akila {
	class System {
	public:
		System() = default;
		virtual ~System() = default;

	protected:
		std::set<Entity> entities;

	private:
		friend class SystemManager;
		friend class Coordinator;

		Signature signature;

		bool isCompatible(Signature const &s) {
			return (signature & s) == signature;
		}

		void setSignature(Signature const &s) {
			signature = s;
		}

		void eraseEntity(EntityId entityId) {
			entities.erase(entityId);
		}

		void addIfCompatible(EntityId entityId, Signature const &s) {
			if(isCompatible(s)) {
				entities.insert(entityId);
			}
		}

		void eraseIfNotCompatible(EntityId entityId, Signature const &s) {
			if(!isCompatible(s)) {
				eraseEntity(entityId);
			}
		}
	};
}
