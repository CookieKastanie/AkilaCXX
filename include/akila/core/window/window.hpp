#pragma once

#pragma warning(push, 0)
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#pragma warning(pop)

#include "akila/core/math/math.hpp"
#include <limits>

namespace akila {
	namespace internal {
		class ImGuiHandler;
	}

	class Window {
	public:
		struct InitValues {
			IVec2 size{640, 480};
			IVec2 position{std::numeric_limits<int>::min(), std::numeric_limits<int>::min()};
			std::string title = "Akila";
			bool decoaration = true;
			bool visible = true;
			bool vSync = true;
			int samples = 0;
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
		friend class Cursor;

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
