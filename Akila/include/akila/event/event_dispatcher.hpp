#pragma once

#include "akila/event/event_listener.hpp"

namespace akila {
	template<typename T>
	class EventDispatcher {
	public:
		template<typename ...Args>
		EventDispatcher(Args&& ...args):
			data{std::forward<Args>(args)...} {};

		inline void dispatch(std::vector<EventListener<T>> &listeners) {
			for(EventListener<T> &el : listeners) el.call(data);
		};

	private:
		T data;
	};
}
