#pragma once

#include <memory>

namespace Akila {
	class State {
		public:
			State();
			virtual ~State();

			virtual void update();
			virtual void draw();
	};

	class StateManager {
		private:
			std::shared_ptr<State> currentState;

		public:
			StateManager();

			void setState(std::shared_ptr<State> state);
			void setState(State *state);

			State *getCurrentState();
	};
}