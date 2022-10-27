#include "akila/signal/listener.hpp"
#include "akila/signal/signals.hpp"

using namespace akila;

Listener::Listener(): type{0}, id{0} {};

Listener::Listener(TypeId type, std::size_t id): type{type}, id{id} {};

Listener::Listener(Listener &&l) noexcept {
	bool reset = type == 0;

	type = l.type;
	id = l.id;

	if(reset) l.type = 0;
};

Listener &Listener::operator=(Listener &&l) noexcept {
	bool reset = type == 0;

	type = l.type;
	id = l.id;

	if(reset) l.type = 0;

	return *this;
};

Listener::~Listener() {
	if(type != 0) Signals::removeListener(*this);
};
