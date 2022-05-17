#include "akila/signal/signals.hpp"

using namespace akila;

std::vector<std::unique_ptr<ISignalQueue>> Signals::allQueues;
std::vector<ISignalQueue *> Signals::queues[3];
std::unordered_map<TypeId, std::size_t> Signals::typeToIndex;
