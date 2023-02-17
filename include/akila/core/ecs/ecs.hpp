#pragma once

#include "akila/core/ecs/coordinator.hpp"
#include "akila/core/ecs/signature.hpp"
#include "akila/core/ecs/entity.hpp"
#include "akila/core/ecs/system.hpp"

namespace akila {
	class ECS {
	public:
		static Entity createEntity() {
			return internal::Coordinator::createEntity();
		}

		static Entity createEntity(Signature const &signature) {
			return internal::Coordinator::createEntity(signature);
		}

		static void addToEraseQueue(Entity entity) {
			internal::Coordinator::addEntityToEraseQueue(entity);
		}

		static void flushEraseQueue() {
			internal::Coordinator::flushEntityEraseQueue();
		}

		static void eraseEntity(Entity const &entity) {
			return internal::Coordinator::eraseEntity(entity);
		}

		template<typename T>
		static T *createSystem() {
			return internal::Coordinator::createSystem<T>();
		}

		template<typename T>
		static T *getSystem() {
			return internal::Coordinator::getSystem<T>();
		}

		template<typename T>
		static void eraseSystem() {
			internal::Coordinator::eraseSystem<T>();
		}

		template<typename ...Ts>
		static Signature createSignature() {
			return internal::Coordinator::createSignature<Ts...>();
		}

		static void resetAll() {
			internal::Coordinator::eraseAllSystem();
			internal::Coordinator::eraseAllEntities();
		}

		static std::unordered_map<TypeId, std::string> const &componentTypeListing() {
			return internal::Coordinator::componentListing();
		}

	private:
		friend class Core;

		static void init() {
			internal::Coordinator::init();
		}

		static void terminate() {
			resetAll();
			internal::Coordinator::terminate();
		}
	};
}
