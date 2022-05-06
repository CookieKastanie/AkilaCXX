#pragma once

#include "akila/ecs/entity_manager.hpp"
#include "akila/ecs/component_manager.hpp"
#include "akila/ecs/system_manager.hpp"

#include <memory>
#include <set>

namespace akila {
	class Coordinator {
	private:
		friend class ECS;
		friend class Entity;

		static std::unique_ptr<EntityManager> entityManager;
		static std::unique_ptr<ComponentManager> componentManager;
		static std::unique_ptr<SystemManager> systemManager;

		static std::set<EntityId> entityEraseQueue;

		static inline EntityId createEntity() {
			return entityManager->create();
		}

		static EntityId createEntity(Signature const &signature) {
			EntityId entityId = entityManager->create();
			entityManager->setSignature(entityId, signature);

			// ajoute tous les components de la signature
			for(ComponentType type = 0; type < MAX_COMPONENT_TYPES; ++type) {
				if(signature[type]) {
					componentManager->addComponent(entityId, type);
				}
			}

			systemManager->addIfCompatible(entityId, signature);

			return entityId;
		}

		static void eraseEntity(EntityId id) {
			// retire un par un les components de l'entity...
			Signature &signature = entityManager->getSignature(id);
			for(ComponentType type = 0; type < signature.size(); ++type) {
				if(signature[type]) {
					componentManager->removeComponent(id, type);
				}
			}

			// puis suppression de l'entity dans les autres managers
			systemManager->erase(id, signature);
			entityManager->erase(id);
		}

		static inline void addEntityToEraseQueue(EntityId entityId) {
			entityEraseQueue.insert(entityId);
		}

		static inline void flushEntityEraseQueue() {
			for(EntityId entityId : entityEraseQueue) eraseEntity(entityId);
			entityEraseQueue.clear();
		}

		//---

		template<typename T>
		static void addComponentToEntity(EntityId id, T const &data) {
			//auto create si le type de component n'existe pas
			ComponentType type = componentManager->createIfNeededAndAddComponent<T>(id, data);
			Signature &signature = entityManager->getSignature(id);
			signature[type] = true;

			systemManager->addIfCompatible(id, signature);
		}

		template<typename T>
		static inline T &getEntityComponent(EntityId id) {
			return componentManager->getComponent<T>(id);
		}

		template<typename T>
		static inline bool entityHasComponent(EntityId id) {
			return componentManager->hasComponent<T>(id);
		}

		template<typename T>
		static void removeComponentFromEntity(EntityId id) {
			ComponentType type = componentManager->removeComponent<T>(id);
			Signature &signature = entityManager->getSignature(id);
			Signature prevSignature = signature;
			signature[type] = false;

			// met a jour les systems avec la nouvelle signature de l'entity
			systemManager->eraseIfNotCompatible(id, prevSignature, signature);
		}

		//---

		template<typename T>
		static T *createSystem(Signature const &signature) {
			T *system = systemManager->createSystem<T>(signature);

			// regarde toutes les entities existante pour ajouter
			// celles compatibles au nouveau system
			for(EntityId entityId : entityManager->createdEntityIds) {
				system->addIfCompatible(entityId, entityManager->getSignature(entityId));
			}

			return system;
		}

		template<typename T>
		static inline T *getSystem() {
			return systemManager->getSystem<T>();
		}

		template<typename T>
		static inline void eraseSystem() {
			return systemManager->eraseSystem<T>();
		}

		//---

		template<class T>
		static void addToSignature(Signature &signature) {
			//auto create si le type de component n'existe pas
			ComponentType type = componentManager->createIfNeededAndGetComponentType<T>();
			signature[type] = true;
		}

		template<typename ...Ts>
		static Signature createSignature() {
			Signature signature = 0x0;
			int _[] = {0, (addToSignature<Ts>(signature), 0)...}; (void)_; //wtf
			return signature;
		}
	};
}
