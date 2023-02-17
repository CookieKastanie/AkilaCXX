#pragma once

#include "akila/core/signals/signal_listener.hpp"

namespace akila {
	namespace internal {
		template<typename T>
		class SignalDispatcher {
		public:
			template<typename ...Args>
			SignalDispatcher(Args&& ...args):
				data{std::forward<Args>(args)...} {};

			void dispatch(std::vector<SignalListener<T>> &listeners) {
				for(SignalListener<T> &el : listeners) el.call(data);
			};

		private:
			T data;
		};
	}
}
