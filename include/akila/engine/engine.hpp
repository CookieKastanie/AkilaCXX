#pragma once

#include "akila/core/core.hpp"

namespace akila {
	class Engine {
	public:
		static int run(Core::InitValues const &initVals, std::function<void()> init);
		static int run(std::function<void()> init);
		static void restart();
	};
}
