#pragma once

#include "akila/ecs/coordinator.hpp"
#include "akila/ecs/signature.hpp"
#include "akila/ecs/entity.hpp"
#include "akila/ecs/system.hpp"

namespace akila {
	class ECS {
	public:
		static inline Entity createEntity() {
			return internal::Coordinator::createEntity();
		}

		static inline Entity createEntity(Signature const &signature) {
			return internal::Coordinator::createEntity(signature);
		}

		static inline void addToEraseQueue(Entity entity) {
			internal::Coordinator::addEntityToEraseQueue(entity);
		}

		static inline void flushEraseQueue() {
			internal::Coordinator::flushEntityEraseQueue();
		}

		static inline void eraseEntity(Entity const &entity) {
			return internal::Coordinator::eraseEntity(entity);
		}

		template<typename T>
		static inline T *createSystem() {
			return internal::Coordinator::createSystem<T>();
		}

		template<typename T>
		static inline T *getSystem() {
			return internal::Coordinator::getSystem<T>();
		}

		template<typename T>
		static inline void eraseSystem() {
			internal::Coordinator::eraseSystem<T>();
		}

		template<typename ...Ts>
		static inline Signature createSignature() {
			return internal::Coordinator::createSignature<Ts...>();
		}

		static inline void resetAll() {
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
