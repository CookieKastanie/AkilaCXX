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
		System() = default;
		virtual ~System() = default;

	protected:
		std::set<Entity> entities;

	private:
		friend class internal::SystemManager;
		friend class internal::Coordinator;

		Signature signature;

		bool isCompatible(Signature const &s) {
			return (signature & s) == signature;
		}

		void setSignature(Signature const &s) {
			signature = s;
		}

		void eraseEntity(internal::EntityId entityId) {
			entities.erase(entityId);
		}

		void addIfCompatible(internal::EntityId entityId, Signature const &s) {
			if(isCompatible(s)) {
				entities.insert(entityId);
			}
		}

		void eraseIfNotCompatible(internal::EntityId entityId, Signature const &s) {
			if(!isCompatible(s)) {
				eraseEntity(entityId);
			}
		}
	};
}
