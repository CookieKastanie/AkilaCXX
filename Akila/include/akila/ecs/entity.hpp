#pragma once

#include "akila/ecs/entity_id.hpp"
#include "akila/ecs/coordinator.hpp"

namespace akila {
	namespace internal {
		class Coordinator;
	}

	class Entity {
	public:
		template<typename T>
		inline void addComponent(T const &data = T{}) {
			internal::Coordinator::addComponentToEntity<T>(id, data);
		}

		template<typename T>
		inline T &getComponent() {
			return internal::Coordinator::getEntityComponent<T>(id);
		}

		template<typename T>
		inline void removeComponent() {
			internal::Coordinator::removeComponentFromEntity<T>(id);
		}

		template<typename T>
		inline bool hasComponent() const {
			return internal::Coordinator::entityHasComponent<T>(id);
		}

		constexpr inline operator internal::EntityId() const {
			return id;
		}

	private:
		friend class ECS;
		friend class internal::Coordinator;
		friend class System;

		internal::EntityId id;
		inline Entity(internal::EntityId id): id{id} {}
	};
}
