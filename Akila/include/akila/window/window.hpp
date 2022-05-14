#pragma once

//#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "akila/math/math.hpp"

namespace akila {
	class Window {
	public:
		static void close();

		static void setSize(IVec2 const &size);
		static IVec2 getSize();

		static void setPosition(IVec2 const &pos);
		static IVec2 getPosition();

		static void setLimits(IVec2 const &minSize, IVec2 const &maxSize);
		
		static void setTitle(std::string const &title);

		static void setVerticalSync(bool b);
		static bool isVerticalSyncEnabled();

		static bool isFullscreen();
		static void setFullscreen(bool fullscreen);

	private:
		friend class Core;

		static GLFWwindow *window;
		static bool vSync;

		static void initWindow();
		static void iniGraphicContext();
		static void makeCurrent();
		static bool shouldClose();
		static void swapBuffers();
		static void terminate();
	};
}
