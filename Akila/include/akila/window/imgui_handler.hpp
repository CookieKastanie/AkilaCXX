#pragma once

#include <imgui/imgui.h>

namespace akila::internal {
	class ImGuiHandler {
		private:
			friend class Core;

			static ImGuiIO *io;

			static void init();

			static void beginFrame();
			static void endFrame();

			static void terminate();
	};
}
