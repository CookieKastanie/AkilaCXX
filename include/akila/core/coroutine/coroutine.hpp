#pragma once

#include <memory>
#include <functional>
#include "akila/core/time/time.hpp"

struct mco_coro;
namespace akila {
	class Coroutine {
	public:
		class Context {
		public:
			void yield() const;
			void wait(float delta) const;

			~Context();

		private:
			friend class Coroutine;
			
			mutable float delay;
			mutable bool waiting;
			mco_coro *mco;
			std::function<void(Context const &)> function;

			Context();
			void deleteMCO();
		};

		Coroutine();

		Coroutine(const Coroutine &) = delete;
		Coroutine(Coroutine &&) = delete;
		Coroutine &operator=(const Coroutine &) = delete;
		Coroutine &operator=(Coroutine &&) = delete;

		using Function = std::function<void(Context const &)>;
		void start(std::size_t stackSize, Function const &function);
		void start(Function const &function);
		bool isAlive();
		void resume(float delta = Time::delta);
		void kill();

	private:
		std::unique_ptr<Context> context;

		static void retrieveCoroFunc(mco_coro *mco);
	};
}
