#pragma once

#include <array>
#include <vector>
#include <unordered_map>
#include <memory>

#include "akila/core/ecs/entity_id.hpp"
#include "akila/core/common/type_infos.hpp"

namespace akila {
	namespace internal {
		using ComponentIndex = EntityId; // il ne peut pas y avoir plus d'index de component que d'entity
		const ComponentIndex NULL_COMPONENT_INDEX = static_cast<ComponentIndex>(-1);

		using ComponentType = std::uint8_t;
		const ComponentType MAX_COMPONENT_TYPES = 64;

		class IComponentVector {
		protected:
			friend class ComponentManager;

			std::vector<EntityId> componentIndexToEntityId;
			std::array<ComponentIndex, MAX_ENTITY_COUNT> entityIdToComponentIndex;

			ComponentType type;
			IComponentVector(ComponentType type): type{type}, entityIdToComponentIndex{} {
				for(auto &v : entityIdToComponentIndex) v = NULL_COMPONENT_INDEX;
			}

			virtual void erase(EntityId entityId) = 0;
			virtual void pushEmpty(EntityId entityId) = 0;
		
			bool entityHasComponent(EntityId entityId) {
				return entityIdToComponentIndex[entityId] != NULL_COMPONENT_INDEX;
			}

		public:
			ComponentType getType() {
				return type;
			}

			virtual ~IComponentVector() = default;
		};

		template<typename T>
		class ComponentVector: public IComponentVector {
		private:
			friend class ComponentManager;

			std::vector<T> data;

			ComponentVector(ComponentType type): IComponentVector{type} {}

			T &getDataByEntityId(EntityId entityId) {
				return data[entityIdToComponentIndex[entityId]];
			}

			void push(EntityId entityId, T const &value) {
				if(entityIdToComponentIndex[entityId] != NULL_COMPONENT_INDEX) {
					std::cerr << "Can't push same component to entity !" << std::endl;
					return;
				}

				ComponentIndex componentIndex = static_cast<ComponentIndex>(data.size());
				data.push_back(value);
				componentIndexToEntityId.push_back(entityId);
				entityIdToComponentIndex[entityId] = componentIndex;
			}

			void pushEmpty(EntityId entityId) override {
				push(entityId, T{});
			}

			void erase(EntityId entityId) override {
				ComponentIndex componentIndex = entityIdToComponentIndex[entityId];
				if(componentIndex == NULL_COMPONENT_INDEX) {
					std::cerr << "Can't delete component from entity who doesn't have it !" << std::endl;
					return;
				}

				// copie le dernier element a l'emplacement de l'element a supprimer,
				// puis pop la liste
				ComponentIndex lastComponentIndex = static_cast<ComponentIndex>(data.size() - 1);

				data[componentIndex] = std::move(data[lastComponentIndex]);

				EntityId entityIdToMove = componentIndexToEntityId[lastComponentIndex];
				componentIndexToEntityId[componentIndex] = entityIdToMove;
				entityIdToComponentIndex[entityIdToMove] = componentIndex;

				entityIdToComponentIndex[entityId] = NULL_COMPONENT_INDEX;

				data.pop_back();
				componentIndexToEntityId.pop_back();
			}
		};

		class ComponentManager {
		private:
			friend class Coordinator;

			ComponentType nextType = 0;

			std::unordered_map<TypeId, std::unique_ptr<IComponentVector>> componentVectors;
			std::unordered_map<ComponentType, TypeId> componentTypeToId;

			// mapNames uniquement pour le listing
			std::unordered_map<TypeId, std::string> typeNames;

			template<typename T>
			ComponentVector<T> *createComponent() {
				ComponentType type = nextType++;
				if(nextType >= MAX_COMPONENT_TYPES) {
					std::cerr << "Too many component types !!!" << std::endl;
					return nullptr;
				}

				TypeId id = getTypeId<T>();

				ComponentVector<T> *vector = new ComponentVector<T>{type};
				componentVectors[id] = std::unique_ptr<ComponentVector<T>>(vector);
				componentTypeToId[type] = id;

				// pour le listing
				typeNames[type] = getTypeName<T>();

				return vector;
			}

			template<typename T>
			ComponentType createIfNeededAndAddComponent(EntityId entityId, T const &data) {
				TypeId id = getTypeId<T>();

				ComponentVector<T> *vector = nullptr;

				auto it = componentVectors.find(id);
				if(it == componentVectors.end()) {
					vector = createComponent<T>();
				} else {
					vector = static_cast<ComponentVector<T> *>(it->second.get());
				}

				vector->push(entityId, data);

				return vector->getType();
			}

			template<typename T>
			ComponentType createIfNeededAndGetComponentType() {
				TypeId id = getTypeId<T>();

				ComponentVector<T> *vector = nullptr;

				auto it = componentVectors.find(id);
				if(it == componentVectors.end()) {
					vector = createComponent<T>();
				} else {
					vector = static_cast<ComponentVector<T> *>(it->second.get());
				}

				return vector->getType();
			}

			ComponentType addComponent(EntityId entityId, ComponentType type) {
				IComponentVector *vector = componentVectors.at(componentTypeToId[type]).get();
				vector->pushEmpty(entityId);
				return vector->getType();
			}

			template<typename T>
			bool hasComponent(EntityId entityId) {
				TypeId id = getTypeId<T>();

				auto it = componentVectors.find(id);
				if(it == componentVectors.end()) return false;

				return it->second->entityHasComponent(entityId);
			}

			template<typename T>
			T &getComponent(EntityId entityId) {
				TypeId id = getTypeId<T>();
				IComponentVector *vector = componentVectors.at(id).get();
				return static_cast<ComponentVector<T>*>(vector)->getDataByEntityId(entityId);
			}

			template<typename T>
			ComponentType removeComponent(EntityId entityId) {
				TypeId id = getTypeId<T>();

				IComponentVector* vector = componentVectors.at(id).get();
				vector->erase(entityId);

				return vector->getType();
			}

			ComponentType removeComponent(EntityId entityId, ComponentType type) {
				TypeId id = componentTypeToId[type];

				IComponentVector *vector = componentVectors.at(id).get();
				vector->erase(entityId);

				return vector->getType();
			}

			std::unordered_map<TypeId, std::string> const &typesListing() {
				return typeNames;
			}
		};
	}
}
