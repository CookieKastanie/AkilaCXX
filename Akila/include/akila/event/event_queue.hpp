#pragma once

#include "akila/event/listener.hpp"
#include "akila/event/event_listener.hpp"
#include "akila/event/event_callback.hpp"
#include "akila/common/type_infos.hpp"
#include <vector>

namespace akila {
	class IEventQueue {
	public:
		virtual ~IEventQueue() = default;

	protected:
		friend class Events;

		inline ListenerId getListenerId(Listener const &l) {
			return l.id;
		}

		inline Listener createListener(TypeId typeId, ListenerId id) {
			return {typeId, id};
		}

		virtual void removeListener(Listener const &listener) = 0;
		virtual void flush() = 0;
	};

	template<typename T>
	class EventQueue: public IEventQueue {
	public:
		EventQueue(TypeId typeId): typeId{typeId}, dipatchers{}, dispatchersIndex{0} {}
		
		Listener addListener(EventCallback<T> const &callback) {
			EventListener<T> el{callback};
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
		void inline enqueue(Args&& ... args) {
			dipatchers[dispatchersIndex].push_back(EventDispatcher<T>{std::forward<Args>(args)...});
		}

		void flush() override {
			auto &dispatcher = dipatchers[dispatchersIndex];
			dispatchersIndex = !dispatchersIndex;

			for(EventDispatcher<T> &d : dispatcher) {
				d.dispatch(listeners);
			}

			dispatcher.clear();
		}

	private:
		TypeId typeId;

		std::vector<EventListener<T>> listeners;
		std::vector<EventDispatcher<T>> dipatchers[2];
		char dispatchersIndex;
	};
}
