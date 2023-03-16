#include "akila/core/ecs/system_manager.hpp"
#include "akila/core/ecs/system.hpp"

using namespace akila;

void internal::SystemManager::addIfCompatible(EntityId entityId, Signature const &s) {
	for(auto it = systems.begin(); it != systems.end(); ++it) {
		it->second->addIfCompatible(entityId, s);
	}
}

void internal::SystemManager::erase(EntityId entityId, Signature const &s) {
	for(auto it = systems.begin(); it != systems.end(); ++it) {
		if(it->second->isCompatible(s))
			it->second->eraseEntity(entityId);
	}
}

void internal::SystemManager::eraseIfNotCompatible(EntityId entityId, Signature const &prevSignature, Signature const &newSignature) {
	for(auto it = systems.begin(); it != systems.end(); ++it) {
		if(it->second->isCompatible(prevSignature))
			it->second->eraseIfNotCompatible(entityId, newSignature);
	}
}

void internal::SystemManager::eraseAll() {
	for(auto [key, system] : systems) {
		delete system;
	}

	systems.clear();
}
