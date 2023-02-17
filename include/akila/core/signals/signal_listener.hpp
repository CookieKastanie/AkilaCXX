#pragma once

#include "akila/core/signals/signal_callback.hpp"
#include "akila/core/signals/listener.hpp"

namespace akila {
	namespace internal {
		class SignalListenerIds {
		public:
			static ListenerId nextListenerId;
		};
	

		template<typename T>
		class SignalListener {
		public:
			SignalListener(SignalCallback<T> const &cb): id{SignalListenerIds::nextListenerId++}, callback{cb} {};

			void call(T const &data) {
				callback(data);
			}

			ListenerId getId() {
				return id;
			}

		private:
			ListenerId id;
			SignalCallback<T> callback;
		};
	}
}