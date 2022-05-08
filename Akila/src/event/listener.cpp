#include "akila/event/listener.hpp"
#include "akila/event/events.hpp"

using namespace akila;

Listener::Listener(TypeId type, std::size_t id): type{type}, id{id} {};

Listener::~Listener() {
	Events::removeListener(*this);
};
