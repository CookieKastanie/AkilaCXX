#pragma once

#include "akila/core/ecs/entity_id.hpp"
#include "akila/core/ecs/coordinator.hpp"

namespace akila {
	namespace internal{
		class Coordinator;
	}

	class Entity {
	public:
		Entity(): id{internal::INVALID_ENTITY_ID} {};

		template<typename T>
		T &addComponent(T const &data = T{}) {
			internal::Coordinator::addComponentToEntity<T>(id, data);
			return internal::Coordinator::getEntityComponent<T>(id);
		}

		template<typename T>
		T &getComponent() {
			return internal::Coordinator::getEntityComponent<T>(id);
		}

		template<typename T>
		void removeComponent() {
			internal::Coordinator::removeComponentFromEntity<T>(id);
		}

		template<typename T>
		bool hasComponent() const {
			return internal::Coordinator::entityHasComponent<T>(id);
		}

		bool hasComponents(Signature signature) const {
			return internal::Coordinator::entityHasComponents(id, signature);
		}

		bool isValid() const {
			return id != internal::INVALID_ENTITY_ID;
		}

		operator internal::EntityId() const {
			return id;
		}

		bool operator==(Entity other) const {
			return id == other.id;
		}

	private:
		friend class ECS;
		friend class internal::Coordinator;
		friend class System;

		internal::EntityId id;
		Entity(internal::EntityId id): id{id} {}
	};
}
