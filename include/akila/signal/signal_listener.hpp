#pragma once

#include "akila/signal/signal_callback.hpp"
#include "akila/signal/listener.hpp"

namespace akila::internal {
	class SignalListenerIds {
	public:
		static ListenerId nextListenerId;
	};
	

	template<typename T>
	class SignalListener {
	public:
		SignalListener(SignalCallback<T> const &cb): id{SignalListenerIds::nextListenerId++}, callback{cb} {};

		inline void call(T const &data) {
			callback(data);
		}

		inline ListenerId getId() {
			return id;
		}

	private:
		ListenerId id;
		SignalCallback<T> callback;
	};
}
