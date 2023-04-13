#pragma once

#include "akila/core/ecs/entity_id.hpp"
#include "akila/core/ecs/signature.hpp"
#include "akila/core/common/type_infos.hpp"

#include <unordered_map>
#include <memory>
#include <type_traits>
#include <iostream>

namespace akila {
	class System;

	namespace internal {
		class SystemManager {
		private:
			friend class Coordinator;

			std::unordered_map<TypeId, System*> systems;

			SystemManager() = default;
			~SystemManager() {
				eraseAll();
			}

			SystemManager(SystemManager &&other) = delete;
			SystemManager &operator=(SystemManager &&other) = delete;

			SystemManager(SystemManager const &other) = delete;
			SystemManager &operator=(SystemManager const &other) = delete;

			template<typename T>
			T *createSystem() {
				static_assert(std::is_base_of<System, T>::value, "T must derive from akila::System");

				TypeId id = getTypeId<T>();
				systems[id] = new T();

				System *system = systems.at(id);
				return static_cast<T *>(system);
			}

			template<typename T>
			T *getSystem() {
				TypeId id = getTypeId<T>();
				System *system = systems.at(id);
				return static_cast<T *>(system);
			}

			template<typename T>
			void eraseSystem() {
				TypeId id = getTypeId<T>();

				auto it = systems.find(id);
				if(it != systems.end()) {
					delete it->second;
					systems.erase(it);
				}
			}

			void addIfCompatible(EntityId entityId, Signature const &s);
			void erase(EntityId entityId, Signature const &s);
			void eraseIfNotCompatible(EntityId entityId, Signature const &prevSignature, Signature const &newSignature);
			void eraseAll();
		};
	}
}
