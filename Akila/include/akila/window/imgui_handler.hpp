#pragma once

#include <imgui/imgui.h>

namespace akila::internal {
	class ImGuiHandler {
		private:
			friend class Core;
			friend class WindowEvents;

			static ImGuiIO *io;

			static void init();

			static void beginFrame();
			static void endFrame();

			static bool wantCaptureMouse();

			static void terminate();
	};
}
