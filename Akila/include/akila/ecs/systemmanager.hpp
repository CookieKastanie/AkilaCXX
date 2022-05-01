#pragma once

#include "akila/ecs/system.hpp"

#include <unordered_map>
#include <memory>
#include <type_traits>
#include <iostream>

namespace akila {
	class SystemManager {
	private:
		friend class Coordinator;

		std::unordered_map<TypeName, std::unique_ptr<System>> systems;

		template<typename T>
		T *createSystem(Signature const &signature) {
			if(!std::is_base_of<System, T>()) {
				std::cerr << typeid(T).name() << " must inherit from System" << std::endl;
				return nullptr;
			}

			TypeName name = typeid(T).hash_code();
			systems[name] = std::make_unique<T>();
			systems[name]->setSignature(signature);

			System *system = systems.at(name).get();
			return static_cast<T *>(system);
		}

		template<typename T>
		T *getSystem() {
			TypeName name = typeid(T).hash_code();
			System *system = systems.at(name).get();
			return static_cast<T *>(system);
		}

		template<typename T>
		void eraseSystem() {
			TypeName name = typeid(T).hash_code();
			systems.erase(name);
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
	};
}
