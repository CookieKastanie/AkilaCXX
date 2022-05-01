#pragma once

#include "akila/ecs/entityid.hpp"
#include "akila/ecs/coordinator.hpp"

namespace akila {
	class Entity {
	public:
		template<typename T>
		inline void addComponent(T const &data = T{}) {
			Coordinator::addComponentToEntity<T>(id, data);
		}

		template<typename T>
		inline T &getComponent() {
			return Coordinator::getEntityComponent<T>(id);
		}

		template<typename T>
		inline void removeComponent() {
			Coordinator::removeComponentFromEntity<T>(id);
		}

		//inline void addToEraseQueue() {
		//	Coordinator::addEntityToEraseQueue(id);
		//}

		template<typename T>
		inline bool hasComponent() {
			return Coordinator::entityHasComponent<T>(id);
		}

		constexpr inline operator EntityId() const {
			return id;
		}

	private:
		friend class ECS;
		friend class Coordinator;
		friend class System;

		EntityId id;
		inline Entity(EntityId id): id{id} {}
	};
}
