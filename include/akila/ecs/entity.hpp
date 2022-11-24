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
		T &addComponent(T const &data = T{}) const {
			internal::Coordinator::addComponentToEntity<T>(id, data);
			return internal::Coordinator::getEntityComponent<T>(id);
		}

		template<typename T>
		T &getComponent() const {
			return internal::Coordinator::getEntityComponent<T>(id);
		}

		template<typename T>
		void removeComponent() const {
			internal::Coordinator::removeComponentFromEntity<T>(id);
		}

		template<typename T>
		bool hasComponent() const {
			return internal::Coordinator::entityHasComponent<T>(id);
		}

		bool isValid() const {
			return id != internal::INVALID_ENTITY_ID;
		}

		operator internal::EntityId() const {
			return id;
		}

	private:
		friend class ECS;
		friend class internal::Coordinator;
		friend class System;

		internal::EntityId id;
		Entity(internal::EntityId id): id{id} {}
	};
}
