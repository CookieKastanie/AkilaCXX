#pragma once

#include "akila/common/type_infos.hpp"

namespace akila {
	using ListenerId = std::size_t;

	class Listener {
	public:
		Listener();

		Listener(Listener const &) = delete;
		Listener &operator=(Listener const &) = delete;

		Listener(Listener &&l) noexcept;
		Listener &operator=(Listener &&l) noexcept;

		~Listener();

	private:
		friend class Events;
		friend class IEventQueue;

		TypeId type;
		ListenerId id;
		
		Listener(TypeId type, ListenerId id);
	};
}
