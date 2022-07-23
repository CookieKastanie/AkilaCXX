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

		//glfwSetWindowSizeCallback(window, [](GLFWwindow *window, int width, int height)
		static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
		static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
		static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
		static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);


		struct MouseData {
			Vec2 lastPosition;
			Vec2 position;
			Vec2 velocity;
			Vec2 scrollVelocity;

			void resetVels();
		};

		static MouseData accumulatedMouse;
		static MouseData frameMouse;
		static MouseData interpoledMouse;

		static void process(unsigned int updateCount);

		static void beforeTick();
		static void beforeFrame();
	};
}
