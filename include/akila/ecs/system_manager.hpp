#pragma once

#include "akila/ecs/system.hpp"

#include <unordered_map>
#include <memory>
#include <type_traits>
#include <iostream>

namespace akila {
	namespace internal {
		class SystemManager {
		private:
			friend class Coordinator;

			std::unordered_map<TypeId, std::unique_ptr<System>> systems;

			template<typename T>
			T *createSystem() {
				static_assert(std::is_base_of<System, T>::value, "T must derive from akila::System");

				TypeId id = getTypeId<T>();
				systems[id] = std::make_unique<T>();

				System *system = systems.at(id).get();
				return static_cast<T *>(system);
			}

			template<typename T>
			T *getSystem() {
				TypeId id = getTypeId<T>();
				System *system = systems.at(id).get();
				return static_cast<T *>(system);
			}

			template<typename T>
			void eraseSystem() {
				TypeId id = getTypeId<T>();
				systems.erase(id);
			}

			void addIfCompatible(EntityId entityId, Signature const &s) {
				for(auto it = systems.begin(); it != systems.end(); ++it) {
					it->second->addIfCompatible(entityId, s);
				}
			}

			void erase(EntityId entityId, Signature const &s) {
				for(auto it = systems.begin(); it != systems.end(); ++it) {
					if(it->second->isCompatible(s))
						it->second->eraseEntity(entityId);
				}
			}

			void eraseIfNotCompatible(EntityId entityId, Signature const &prevSignature, Signature const &newSignature) {
				for(auto it = systems.begin(); it != systems.end(); ++it) {
					if(it->second->isCompatible(prevSignature))
						it->second->eraseIfNotCompatible(entityId, newSignature);
				}
			}

			void eraseAll() {
				systems.clear();
			}
		};
	}
}
