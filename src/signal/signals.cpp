#include "akila/signal/signals.hpp"

using namespace akila;

std::vector<std::unique_ptr<internal::ISignalQueue>> Signals::allQueues;
std::vector<internal::ISignalQueue *> Signals::queues[2];
std::unordered_map<TypeId, std::size_t> Signals::typeToIndex;
