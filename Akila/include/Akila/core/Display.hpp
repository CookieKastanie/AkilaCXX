#pragma once

#include "GLFW/glfw3.h"
#include "Akila/inputs/Keyboard.hpp"
#include "Akila/inputs/Mouse.hpp"
#include <functional>
#include <string>

namespace Akila {
	class Display {
		private:
			friend class Core;

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
			std::function<void()> resizeCallback;

		public:
			static int const DONT_CARE = GLFW_DONT_CARE;

			Display();
			~Display();

			void setSize(int w, int h);
			void setLimits(int minW, int minH, int maxW, int maxH);
			void setTitle(const std::string &title);

			int getWidth();
			int getHeight();

			void makeCurrent();

			void setVerticalSync(bool b);
			bool isVerticalSyncEnabled();

			bool shouldClose();
			void close();
			void swapBuffers();

			bool isFullscreen();
			void setFullscreen(bool fullscreen);

			Keyboard *getKeybord();
			Mouse *getMouse();

			void beforePollEvent();

			void onResize(std::function<void()> const &cb);

		private:
			void setRendererResizeCallback(const std::function<void()> &cb);
	};
}