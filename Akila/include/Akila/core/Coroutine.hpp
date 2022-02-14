#pragma once

#include "Akila/core/Memory.hpp"
#include <queue>
#include "Akila/core/ThreadPool.hpp"

namespace Akila {
	enum class Trigger {
		AT_FRAME_START,
		AFTER_FIXED_UPDATE,
		BEFORE_DRAW,
	};

	class CoroutineBase {
		friend class CoroutineManager;

	protected:
		int index;

		struct Infos {
			Trigger trigger;
			bool executeInThread;
		};

		std::vector<Infos> infos;

		CoroutineBase();
		virtual ~CoroutineBase();

		virtual int execute() = 0;

	public:
		void reset();
		void stop();
	};

	template<typename T>
	class Coroutine: public CoroutineBase {
		friend class CoroutineManager;

	private:
		typedef int(*F)(T &);

		T state;
		std::vector<F> steps;

		Coroutine(T const &state): CoroutineBase{}, state{state} {}

		int execute() override {
			return steps[index](state);
		}

	public:
		void push(Trigger trigger, F const &f) {
			infos.push_back({trigger, false});
			steps.push_back(f);
		}

		void pushInThread(Trigger trigger, F const &f) {
			infos.push_back({trigger, true});
			steps.push_back(f);
		}

		void setState(T const &s) {
			state = s;
		}
	};

	class CoroutineManager {
		friend class Core;

	private:
		typedef Ptr<CoroutineBase> QData;

		std::queue<QData> atFrameStart;
		std::queue<QData> afterFixedUpdate;
		std::queue<QData> beforeDraw;

		ThreadPool threadPool;

		CoroutineManager();

		void process(QData c);
		void flush(std::queue<QData> &queue);

		void flushAtFrameStart();
		void flushAfterFixedUpdate();
		void flushBeforeDraw();

	public:
		template<typename T>
		Ptr<Coroutine<T>> create(T const &state) {
			return createPtr(new Coroutine(state));
		}

		template<typename T>
		Ptr<Coroutine<T>> create() {
			return create({});
		}

		void start(Ptr<CoroutineBase> coroutine);
	};
}
