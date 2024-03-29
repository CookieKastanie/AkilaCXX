#pragma once

#include "akila/core/ecs/entity_manager.hpp"
#include "akila/core/ecs/component_manager.hpp"
#include "akila/core/ecs/system_manager.hpp"

#include <set>

namespace akila {
	class ECS;
	class Entity;

	namespace internal {
		class Coordinator {
		private:
			friend class ECS;
			friend class Entity;

			static EntityManager *entityManager;
			static ComponentManager *componentManager;
			static SystemManager *systemManager;

			static std::set<EntityId> entityEraseQueue;

			static void init() {
				entityManager = new EntityManager{};
				componentManager = new ComponentManager{};
				systemManager = new SystemManager{};
				entityEraseQueue = {};
			}

			static void terminate() {
				delete entityManager;
				delete componentManager;
				delete systemManager;
			}

			static EntityId createEntity() {
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
				Signature &signature = entityManager->getSignature(id);

				// retire l'entity des systems
				systemManager->erase(id, signature);

				// retire un par un les components de l'entity
				for(ComponentType type = 0; type < signature.size(); ++type) {
					if(signature[type]) {
						componentManager->removeComponent(id, type);
					}
				}

				// suppression de l'entity
				entityManager->erase(id);
			}

			static void addEntityToEraseQueue(EntityId entityId) {
				entityEraseQueue.insert(entityId);
			}

			static void flushEntityEraseQueue() {
				for(EntityId entityId : entityEraseQueue) eraseEntity(entityId);
				entityEraseQueue.clear();
			}

			static void eraseAllEntities() {
				for(EntityId entityId : entityManager->createdEntityIds) {
					addEntityToEraseQueue(entityId);
				}

				flushEntityEraseQueue();
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
			static T &getEntityComponent(EntityId id) {
				return componentManager->getComponent<T>(id);
			}

			template<typename T>
			static bool entityHasComponent(EntityId id) {
				return componentManager->hasComponent<T>(id);
			}

			static bool entityHasComponents(EntityId id, Signature signature) {
				return (entityManager->getSignature(id) & signature) == signature;
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

			static std::unordered_map<TypeId, std::string> const &componentListing() {
				return componentManager->typesListing();
			}

			//---

			template<typename T>
			static T *createSystem() {
				T *system = systemManager->createSystem<T>();

				// regarde toutes les entities existante pour ajouter
				// celles compatibles au nouveau system
				for(EntityId entityId : entityManager->createdEntityIds) {
					system->addIfCompatible(entityId, entityManager->getSignature(entityId));
				}

				return system;
			}

			template<typename T>
			static T *getSystem() {
				return systemManager->getSystem<T>();
			}

			template<typename T>
			static void eraseSystem() {
				return systemManager->eraseSystem<T>();
			}

			static void eraseAllSystem() {
				systemManager->eraseAll();
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
}
