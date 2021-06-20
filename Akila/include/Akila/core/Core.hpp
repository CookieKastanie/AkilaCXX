#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Akila/core/Display.hpp"
#include "Akila/core/State.hpp"
#include "Akila/core/Task.hpp"

namespace Akila {
	class Core {
		private:
			Core();

		public:
			static Display *display;
			static StateManager *stateManager;
			static TaskManager *taskManager;

			static int run(int argc, char *argv[], void (*init)(void));
	};
}
