#include "akila/ecs/coordinator.hpp"

using namespace akila;

std::unique_ptr<EntityManager> Coordinator::entityManager = std::unique_ptr<EntityManager>(new EntityManager{});
std::unique_ptr<ComponentManager> Coordinator::componentManager = std::unique_ptr<ComponentManager>(new ComponentManager{});
std::unique_ptr<SystemManager> Coordinator::systemManager = std::unique_ptr<SystemManager>(new SystemManager{});

std::set<EntityId> Coordinator::entityEraseQueue{};
