#include "akila/ecs/coordinator.hpp"

using namespace akila;

std::unique_ptr<internal::EntityManager> internal::Coordinator::entityManager = std::unique_ptr<EntityManager>(new EntityManager{});
std::unique_ptr<internal::ComponentManager> internal::Coordinator::componentManager = std::unique_ptr<ComponentManager>(new ComponentManager{});
std::unique_ptr<internal::SystemManager> internal::Coordinator::systemManager = std::unique_ptr<SystemManager>(new SystemManager{});

std::set<internal::EntityId> internal::Coordinator::entityEraseQueue{};
