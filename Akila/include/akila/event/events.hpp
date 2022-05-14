#pragma once

#include "akila/event/listener.hpp"
#include "akila/event/event_dispatcher.hpp"
#include "akila/event/event_queue.hpp"
#include "akila/common/type_infos.hpp"
#include <unordered_map>

namespace akila {
	class Events {
	public:
		enum class Stack: char {
			FRAME_START = 0,
			BEFORE_UPDATE = 1,
			BEFORE_DRAW = 2
			//INSTANT = 3
		};
		
		template<typename T>
		static void registerType(Stack stack = Stack::FRAME_START) {
			TypeId typeId = getTypeId<T>();

			if(typeToIndex.find(typeId) != typeToIndex.end()) {
				std::cerr << "Type " << getTypeName<T>() << " is already registered.";
				return;
			}

			IEventQueue *eventQueue = new EventQueue<T>{typeId};

			queues[static_cast<char>(stack)].push_back(eventQueue);

			std::size_t index = allQueues.size();
			allQueues.push_back(std::unique_ptr<IEventQueue>(eventQueue));
			typeToIndex[typeId] = index;
		}

		template<typename T>
		static Listener listen(EventCallback<T> const &callback) {
			TypeId typeId = getTypeId<T>();
			
			std::size_t index = typeToIndex[typeId];
			EventQueue<T> *q = static_cast<EventQueue<T>*>(allQueues[index].get());
			return q->addListener(callback);
		}

		template<typename T, typename ...Args>
		static void emit(Args&& ...args) {
			TypeId typeId = getTypeId<T>();
			std::size_t index = typeToIndex[typeId];

			EventQueue<T> *q = static_cast<EventQueue<T>*>(allQueues[index].get());
			q->enqueue(std::forward<Args>(args)...);
		}

		static void removeListener(Listener const &listener) {
			std::size_t index = typeToIndex[listener.type];
			allQueues[index]->removeListener(listener);
		}

		static void flush(Stack stack) {
			for(IEventQueue *q : queues[static_cast<char>(stack)]) {
				q->flush();
			}
		}

	private:
		friend class Listener;
		friend class Core;

		static std::vector<std::unique_ptr<IEventQueue>> allQueues;
		static std::vector<IEventQueue*> queues[3];
		static std::unordered_map<TypeId, std::size_t> typeToIndex;
	};
}
