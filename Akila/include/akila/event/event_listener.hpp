#pragma once

#include "akila/event/event_callback.hpp"
#include "akila/event/listener.hpp"

namespace akila {
	class EventListenerIds {
	public:
		static ListenerId nextListenerId;
	};
	

	template<typename T>
	class EventListener {
	public:
		EventListener(EventCallback<T> const &cb): id{EventListenerIds::nextListenerId++}, callback{cb} {};

		inline void call(T const &data) {
			callback(data);
		}

		inline ListenerId getId() {
			return id;
		}

	private:
		ListenerId id;
		EventCallback<T> callback;
	};
}
