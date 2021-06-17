#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Akila/core/Display.hpp"
#include "Akila/core/State.hpp"
#include "Akila/core/Task.hpp"

namespace Akila {
	class Core {
		private:
			static std::shared_ptr<State> currentState;

		public:
			static Display *display;
			static TaskManager *taskManager;

			static int run(int argc, char *argv[], void (*init)(void));
			static void setState(std::shared_ptr<State> state);
			static void setState(State *state);
	};
}
