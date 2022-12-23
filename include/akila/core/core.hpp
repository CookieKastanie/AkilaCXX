#pragma once

#include "akila/core/window/window.hpp"

namespace akila {
	class Core {
	public:
		static int run(Window::InitValues const &initVals, void (*init)(void));
		static int run(void (*init)(void));
		static void restart();

	private:
		static bool restartFlag;
	};
}
