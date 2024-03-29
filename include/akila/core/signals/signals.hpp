#pragma once

#include "akila/core/signals/listener.hpp"
#include "akila/core/signals/signal_dispatcher.hpp"
#include "akila/core/signals/signal_queue.hpp"
#include "akila/core/common/type_infos.hpp"

#include <unordered_map>
#include <iostream>

namespace akila {
	class Signals {
	public:
		enum class Stack: char {
			BEFORE_TICK = 0,
			BEFORE_FRAME = 1
		};
		
		template<typename T>
		static void registerType(Stack stack = Stack::FRAME_START) {
			TypeId typeId = getTypeId<T>();

			if(typeToIndex.find(typeId) != typeToIndex.end()) {
				std::cerr << "Type " << getTypeName<T>() << " is already registered.";
				return;
			}

			internal::ISignalQueue *eventQueue = new internal::SignalQueue<T>{typeId};

			queues[static_cast<char>(stack)].push_back(eventQueue);

			std::size_t index = allQueues.size();
			allQueues.push_back(std::unique_ptr<internal::ISignalQueue>(eventQueue));
			typeToIndex[typeId] = index;
		}

		template<typename T>
		static Listener listen(SignalCallback<T> const &callback) {
			TypeId typeId = getTypeId<T>();

			auto indexIt = typeToIndex.find(typeId);
			if(indexIt == typeToIndex.end()) {
				std::cerr << "Can't listen to type : " << getTypeName<T>() << std::endl;
				return {};
			}

			internal::SignalQueue<T> *q = static_cast<internal::SignalQueue<T>*>(allQueues[indexIt->second].get());
			return q->addListener(callback);
		}

		template<typename T, class F, class E>
		static Listener listen(F &&callback, E &&type) {
			return listen<T>(std::bind(
				std::forward<F>(callback),
				std::forward<E>(type),
				std::placeholders::_1
			));
		}

		template<typename T, typename ...Args>
		static void emit(Args&& ...args) {
			TypeId typeId = getTypeId<T>();
			std::size_t index = typeToIndex[typeId];

			internal::SignalQueue<T> *q = static_cast<internal::SignalQueue<T>*>(allQueues[index].get());
			q->enqueue(std::forward<Args>(args)...);
		}

	private:
		friend class Listener;
		friend class Core;

		static void init() {
			internal::SignalListenerIds::nextListenerId = 0;
		}

		static void terminate() {
			queues[0].clear();
			queues[1].clear();
			allQueues.clear();
			typeToIndex.clear();
		}

		static void removeListener(Listener const &listener) {
			std::size_t index = typeToIndex[listener.type];
			allQueues[index]->removeListener(listener);
		}

		static void flush(Stack stack) {
			for(internal::ISignalQueue *q : queues[static_cast<char>(stack)]) {
				q->flush();
			}
		}

		static std::vector<std::unique_ptr<internal::ISignalQueue>> allQueues;
		static std::vector<internal::ISignalQueue*> queues[2];
		static std::unordered_map<TypeId, std::size_t> typeToIndex;
	};
}
