#pragma once

#include "akila/common/type_infos.hpp"

namespace akila {
	using ListenerId = std::size_t;

	class Listener {
	public:
		Listener(Listener const &) = delete;
		Listener &operator=(Listener const &) = delete;
		Listener(Listener&&) = delete;
		Listener &operator=(Listener &&) = delete;

		~Listener();

	private:
		friend class Events;
		friend class IEventQueue;

		TypeId type;
		ListenerId id;
		
		Listener(TypeId type, ListenerId id);
	};
}
