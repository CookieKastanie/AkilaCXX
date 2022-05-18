#pragma once

#include "akila/signal/listener.hpp"
#include "akila/signal/signal_dispatcher.hpp"
#include "akila/signal/signal_queue.hpp"
#include "akila/common/type_infos.hpp"
#include <unordered_map>

namespace akila {
	class Signals {
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

			ISignalQueue *eventQueue = new SignalQueue<T>{typeId};

			queues[static_cast<char>(stack)].push_back(eventQueue);

			std::size_t index = allQueues.size();
			allQueues.push_back(std::unique_ptr<ISignalQueue>(eventQueue));
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

			SignalQueue<T> *q = static_cast<SignalQueue<T>*>(allQueues[indexIt->second].get());
			return q->addListener(callback);
		}

		template<typename T, typename ...Args>
		static void emit(Args&& ...args) {
			TypeId typeId = getTypeId<T>();
			std::size_t index = typeToIndex[typeId];

			SignalQueue<T> *q = static_cast<SignalQueue<T>*>(allQueues[index].get());
			q->enqueue(std::forward<Args>(args)...);
		}

	private:
		friend class Listener;
		friend class Core;

		static void removeListener(Listener const &listener) {
			std::size_t index = typeToIndex[listener.type];
			allQueues[index]->removeListener(listener);
		}

		static void flush(Stack stack) {
			for(ISignalQueue *q : queues[static_cast<char>(stack)]) {
				q->flush();
			}
		}

		static std::vector<std::unique_ptr<ISignalQueue>> allQueues;
		static std::vector<ISignalQueue*> queues[3];
		static std::unordered_map<TypeId, std::size_t> typeToIndex;
	};
}
