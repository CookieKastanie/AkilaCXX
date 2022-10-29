#pragma once

//#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "akila/math/math.hpp"
#include "akila/window/window_events.hpp"

namespace akila {
	namespace internal {
		class ImGuiHandler;
	}

	class Window {
	public:
		struct InitValues {
			InitValues();

			IVec2 size;
			std::string title;
			bool visible;
			bool vSync;
			int samples;
		};

		static void close();

		static void setSize(IVec2 const &size);
		static IVec2 getSize();

		static void setPosition(IVec2 const &pos);
		static void setPositionToCenter();
		static IVec2 getPosition();

		static void setLimits(IVec2 const &minSize, IVec2 const &maxSize);
		
		static void setTitle(std::string const &title);

		static void setVerticalSync(bool b);
		static bool isVerticalSyncEnabled();

		static bool isFullscreen();
		static void setFullscreen(bool fullscreen);

		static bool isMouseGrabbed();
		static void setMouseGrab(bool grab);

		static bool isVisible();
		static void setVisibility(bool visible);

		static bool isDecoarated();
		static void setDecoaration(bool decoration);

		static float getDPI();

		static void loadImGuiFont(std::string const &path);

	private:
		friend class Core;
		friend class internal::ImGuiHandler;

		static GLFWwindow *window;
		static bool vSync;

		static IVec2 sizeBeforeFS;
		static IVec2 positionBeforeFS;

		static bool mouseGrab;

		static void initWindow(InitValues const &initVals);
		static void initGraphicContext();
		static void makeCurrent();
		static bool shouldClose();
		static void swapBuffers();
		static void terminate();
	};
}
