#pragma once

#include "GLFW/glfw3.h"
#include "Akila/inputs/Keyboard.hpp"
#include "Akila/inputs/Mouse.hpp"
#include <functional>

namespace Akila {
	class Display {
		private:
			GLFWwindow *window;
			int width;
			int height;
			bool vSync;

			int winSize[2];
			int winPos[2];

			Keyboard keybord;
			Mouse mouse;

			friend class Renderer;

			std::function<void()> rendererResizeCallback;

		public:
			static int const DONT_CARE = GLFW_DONT_CARE;

			Display();
			~Display();

			void setSize(int w, int h);
			void setLimits(int minW, int minH, int maxW, int maxH);
			void setTitle(const char *title);

			int getWidth();
			int getHeight();

			void makeCurrent();
			void setVerticalSync(bool b);
			bool shouldClose();
			void swapBuffers();

			bool isFullscreen();
			void setFullscreen(bool fullscreen);

			Keyboard *getKeybord();
			Mouse *getMouse();

		private:
			void setRendererResizeCallback(const std::function<void()> &cb);
	};
}