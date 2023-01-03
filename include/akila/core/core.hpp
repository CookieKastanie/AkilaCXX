#pragma once

#include "akila/core/window/window.hpp"
#include <functional>

namespace akila {
	class Core {
	public:
		static int run(Window::InitValues const &initVals, std::function<void()> init);
		static int run(std::function<void()> init);
		static void restart();

	private:
		static bool restartFlag;
	};
}
