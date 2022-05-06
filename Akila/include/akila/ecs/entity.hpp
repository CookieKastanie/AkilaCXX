#pragma once

#include "akila/ecs/entity_id.hpp"
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

		template<typename T>
		inline bool hasComponent() const {
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
