#pragma once

#include "akila/event/listener.hpp"
#include <functional>
#include <unordered_map>

namespace akila {
	class Events {
	public:
		enum class Stack: char {
			FRAME_START,
			BEFORE_UPDATE,
			BEFORE_DRAW,
			INSTANT
		};

		template<typename T>
		static void registerType(Stack stack) {

		}

		template<typename T>
		static Listener listen(std::function<void()> const &callback) {

		}

		template<typename T, typename ...Args>
		static void emit(Args&& ...args) {

		}

	private:
		friend class Listener;
		friend class Core;
		
		/*/

		class IEventQueue {
		
			
		}

		template<typename T>
		class EventQueue: public IEventQueue {
			std::deque<T> queue;
		}

		//*/

		//static std::unordered_map<TypeId, std::deque<std::unique_ptr<IEventQueue>>> eventQueues;

		static void removeListener(Listener const &listener);
	};
}
