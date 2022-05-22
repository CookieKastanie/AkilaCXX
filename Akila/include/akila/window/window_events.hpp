#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include "akila/inputs/inputs.hpp"
#include <mutex>

namespace akila {
	struct KeyPressSignal {
		Inputs::Key key;
	};

	struct KeyReleaseSignal {
		Inputs::Key key;
	};

	class Window;
	class Core;
}

namespace akila::internal {
	class WindowEvents {
	private:
		friend class akila::Window;
		friend class akila::Core;

		static std::vector<KeyPressSignal> keyPressed;
		static std::vector<KeyReleaseSignal> keyReleased;

		static std::mutex mux;

		static void init();

		static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

		static void emitSignals();
	};
}
