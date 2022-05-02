#pragma once

#include "akila/ecs/coordinator.hpp"
#include "akila/ecs/signature.hpp"
#include "akila/ecs/entity.hpp"
#include "akila/ecs/system.hpp"

namespace akila {
	class ECS {
	public:
		static inline Entity createEntity() {
			return Coordinator::createEntity();
		}

		static inline Entity createEntity(Signature const &signature) {
			return Coordinator::createEntity(signature);
		}

		static void addToEraseQueue(Entity entity) {
			Coordinator::addEntityToEraseQueue(entity);
		}

		static void flushEraseQueue() {
			Coordinator::flushEntityEraseQueue();
		}

		static inline void eraseEntity(Entity const &entity) {
			return Coordinator::eraseEntity(entity);
		}

		template<typename T>
		static inline T *createSystem(Signature const &signature) {
			return Coordinator::createSystem<T>(signature);
		}

		template<typename T>
		static inline T *getSystem() {
			return Coordinator::getSystem<T>();
		}

		template<typename T>
		static inline void eraseSystem() {
			Coordinator::eraseSystem<T>();
		}

		template<typename ...Ts>
		static inline Signature createSignature() {
			return Coordinator::createSignature<Ts...>();
		}
	};
}
