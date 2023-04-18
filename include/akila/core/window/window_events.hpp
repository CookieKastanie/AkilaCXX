#pragma once

#include "akila/core/window/window.hpp"
#include <vector>
#include "akila/core/inputs/inputs.hpp"
#include <mutex>

namespace akila {
	struct WindowResizeSignal {
		IVec2 size;
	};

	struct KeyboardSignal {
		Keyboard::Key key;
		enum class Action {
			PRESS = GLFW_PRESS,
			RELEASE = GLFW_RELEASE
		} action;
	};

	struct MouseButtonSignal {
		Mouse::Button key;
		enum class Action {
			PRESS = GLFW_PRESS,
			RELEASE = GLFW_RELEASE
		} action;
	};

	struct ControllerSignal {
		Controller::Guid guid;

		enum class Type {
			GAMEPAD,
			JOYSTICK
		} type;

		enum class State {
			CONNECTED = GLFW_CONNECTED,
			DISCONNECTED = GLFW_DISCONNECTED,
			RECONNECTED = (GLFW_DISCONNECTED + 1)
		} state;
	};

	class Window;
	class Core;
}

namespace akila {
	namespace internal {
		class WindowEvents {
		private:
			friend class akila::Window;
			friend class akila::Core;

			static std::vector<WindowResizeSignal> windowResizeSignals;
			static std::vector<KeyboardSignal> keyboardSignals;
			static std::vector<MouseButtonSignal> mouseButtonSignals;
			static std::vector<ControllerSignal> controllerSignals;

			static std::mutex mux;

			static void init();
			static void terminate();

			static void resizeCallback(GLFWwindow *window, int width, int height);
			static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
			static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
			static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
			static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

			static void joystickCallback(int jid, int e);

			struct MouseData {
				Vec2 lastPosition{0.f};
				Vec2 position{0.f};
				Vec2 velocity{0.f};

				Vec2 lastScrollPosition{0.f};
				Vec2 scrollPosition{0.f};
				Vec2 scrollVelocity{0.f};

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
}
