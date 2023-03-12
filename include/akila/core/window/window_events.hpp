#pragma once

#include "akila/core/window/window.hpp"
#include <vector>
#include "akila/core/inputs/inputs.hpp"
#include <mutex>

namespace akila {
	struct WindowResizeSignal {
		IVec2 size;
	};

	struct KeyPressSignal {
		Inputs::Key key;
	};

	struct KeyReleaseSignal {
		Inputs::Key key;
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

			static std::vector<WindowResizeSignal> resizes;
			static std::vector<KeyPressSignal> keyPressed;
			static std::vector<KeyReleaseSignal> keyReleased;

			static std::mutex mux;

			static void init();

			static void resizeCallback(GLFWwindow *window, int width, int height);
			static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
			static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
			static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos);
			static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

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
