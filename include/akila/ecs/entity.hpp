#pragma once

#include "akila/ecs/entity_id.hpp"
#include "akila/ecs/coordinator.hpp"

namespace akila {
	namespace internal {
		class Coordinator;
	}

	class Entity {
	public:
		Entity(): id{internal::INVALID_ENTITY_ID} {};

		template<typename T>
		inline void addComponent(T const &data = T{}) const {
			internal::Coordinator::addComponentToEntity<T>(id, data);
		}

		template<typename T>
		inline T &getComponent() const {
			return internal::Coordinator::getEntityComponent<T>(id);
		}

		template<typename T>
		inline void removeComponent() const {
			internal::Coordinator::removeComponentFromEntity<T>(id);
		}

		template<typename T>
		inline bool hasComponent() const {
			return internal::Coordinator::entityHasComponent<T>(id);
		}

		inline bool isValid() const {
			return id != internal::INVALID_ENTITY_ID;
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
