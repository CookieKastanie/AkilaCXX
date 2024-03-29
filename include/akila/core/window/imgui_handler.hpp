#pragma once

#pragma warning(push, 0)
#include <imgui/imgui.h>
#pragma warning(pop)

#include "akila/core/math/math.hpp"

namespace akila::internal {
	class ImGuiHandler {
		static void fastTheming(Vec3 textColor, Vec3 headColor, Vec3 areaColor, Vec3 bodyColor, Vec3 popsColor);

		private:
			friend class Core;
			friend class Window;
			friend class WindowEvents;

			static ImGuiIO *io;
			static std::string iniFilename;

			static void init();
			static void initIniFileName();

			static void beginFrame();
			static void endFrame();

			static bool wantCaptureMouse();

			static void loadFont(std::string const &path, float dpi = 1);

			static void terminate();
	};
}
