#pragma once

#include "akila/signal/signal_listener.hpp"

namespace akila::internal {
	template<typename T>
	class SignalDispatcher {
	public:
		template<typename ...Args>
		SignalDispatcher(Args&& ...args):
			data{std::forward<Args>(args)...} {};

		inline void dispatch(std::vector<SignalListener<T>> &listeners) {
			for(SignalListener<T> &el : listeners) el.call(data);
		};

	private:
		T data;
	};
}
