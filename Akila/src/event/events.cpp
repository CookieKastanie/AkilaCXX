#include "akila/event/events.hpp"

using namespace akila;

std::vector<std::unique_ptr<IEventQueue>> Events::allQueues;
std::vector<IEventQueue *> Events::queues[3];
std::unordered_map<TypeId, std::size_t> Events::typeToIndex;
