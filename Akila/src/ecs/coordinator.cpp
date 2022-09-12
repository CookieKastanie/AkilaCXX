#include "akila/ecs/coordinator.hpp"

using namespace akila;

internal::EntityManager *internal::Coordinator::entityManager = nullptr;
internal::ComponentManager *internal::Coordinator::componentManager = nullptr;
internal::SystemManager *internal::Coordinator::systemManager = nullptr;

std::set<internal::EntityId> internal::Coordinator::entityEraseQueue;
