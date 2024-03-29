#pragma once

#include "akila/core/signals/listener.hpp"
#include "akila/core/signals/signal_listener.hpp"
#include "akila/core/signals/signal_callback.hpp"
#include "akila/core/common/type_infos.hpp"
#include <vector>

namespace akila {
	namespace internal {
		class ISignalQueue {
		public:
			virtual ~ISignalQueue() = default;

			virtual void removeListener(Listener const &listener) = 0;
			virtual void flush() = 0;

		protected:
			friend class Signals;

			ListenerId getListenerId(Listener const &l) {
				return l.id;
			}

			Listener createListener(TypeId typeId, ListenerId id) {
				return {typeId, id};
			}
		};

		template<typename T>
		class SignalQueue: public ISignalQueue {
		public:
			SignalQueue(TypeId typeId): typeId{typeId}, dipatchers{}, dispatchersIndex{0} {}
		
			Listener addListener(SignalCallback<T> const &callback) {
				SignalListener<T> el{callback};
				listeners.push_back(el);
				return createListener(typeId, el.getId());
			}

			void removeListener(Listener const &listener) override {
				for(auto it = listeners.begin(); it != listeners.end(); ++it) {
					if(it->getId() == getListenerId(listener)) {
						listeners.erase(it);
						break;
					}
				}
			}

			template<typename ...Args>
			void enqueue(Args&& ... args) {
				dipatchers[dispatchersIndex].push_back(SignalDispatcher<T>{std::forward<Args>(args)...});
			}

			void flush() override {
				auto &dispatcher = dipatchers[dispatchersIndex];
				dispatchersIndex = !dispatchersIndex;

				for(SignalDispatcher<T> &d : dispatcher) {
					d.dispatch(listeners);
				}

				dispatcher.clear();
			}

		private:
			TypeId typeId;

			std::vector<SignalListener<T>> listeners;
			std::vector<SignalDispatcher<T>> dipatchers[2];
			bool dispatchersIndex;
		};
	}
}
