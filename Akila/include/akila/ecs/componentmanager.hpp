#pragma once

#include <array>
#include <vector>
#include <unordered_map>
#include <memory>

#include "akila/ecs/entity.hpp"
#include "akila/ecs/typename.hpp"

namespace akila {
	using ComponentIndex = EntityId; // il ne peut pas y avoir plus d'index de component que d'entity
	const ComponentIndex NULL_COMPONENT_INDEX = -1;

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
		inline ComponentType getType() {
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

		inline T &getDataByEntityId(EntityId entityId) {
			return data[entityIdToComponentIndex[entityId]];
		}

		void push(EntityId entityId, T const &value) {
			if(entityIdToComponentIndex[entityId] != NULL_COMPONENT_INDEX) {
				std::cerr << "Can't push same component to entity !" << std::endl;
				return;
			}

			ComponentIndex componentIndex = data.size();
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
			ComponentIndex lastComponentIndex = data.size() - 1;

			data[componentIndex] = data[lastComponentIndex];

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

		std::unordered_map<TypeName, std::unique_ptr<IComponentVector>> componentVectors;
		std::unordered_map<ComponentType, TypeName> componentTypeToName;

		template<typename T>
		void createComponent() {
			ComponentType type = nextType++;
			if(nextType >= MAX_COMPONENT_TYPES) {
				std::cerr << "Too many component types !!!" << std::endl;
				return;
			}

			TypeName name = GET_TYPE_NAME(T);
			componentVectors[name] = std::unique_ptr<ComponentVector<T>>(new ComponentVector<T>{type});
			componentTypeToName[type] = name;
		}

		template<typename T>
		ComponentType addComponent(EntityId entityId, T const &data) {
			TypeName name = GET_TYPE_NAME(T);

			IComponentVector *vector = componentVectors.at(name).get();
			static_cast<ComponentVector<T> *>(vector)->push(entityId, data);

			return vector->getType();
		}

		ComponentType addComponent(EntityId entityId, ComponentType type) {
			IComponentVector *vector = componentVectors.at(componentTypeToName[type]).get();
			vector->pushEmpty(entityId);
			return vector->getType();
		}

		template<typename T>
		bool hasComponent(EntityId entityId) {
			TypeName name = GET_TYPE_NAME(T);

			auto &it = componentVectors.find(name);
			if(it == componentVectors.end()) return false;

			return it->second->entityHasComponent(entityId);
		}

		template<typename T>
		T &getComponent(EntityId entityId) {
			TypeName name = GET_TYPE_NAME(T);
			IComponentVector *vector = componentVectors.at(name).get();
			return static_cast<ComponentVector<T>*>(vector)->getDataByEntityId(entityId);
		}

		ComponentType removeComponent(EntityId entityId, ComponentType type) {
			TypeName name = componentTypeToName[type];

			IComponentVector *vector = componentVectors.at(name).get();
			vector->erase(entityId);

			return vector->getType();
		}
	};
}
