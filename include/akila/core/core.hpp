#pragma once

#include "akila/core/window/window.hpp"
#include "akila/core/threadpool/threadpool.hpp"
#include "akila/core/inputs/inputs.hpp"
#include "akila/core/resources/file_system.hpp"
#include <functional>

namespace akila {
	class Core {
	public:
		struct InitValues{
			Window::InitValues window{};
			FileSystem::InitValues fileSystem{};
			Inputs::InitValues inputs{};
			Threadpool::InitValues threadpool{};
			bool useSystemEventsThread = false;
		};

		static InitValues parseArguments(int argc, char *argv[]);
		static void parseArguments(InitValues &initValues, int argc, char *argv[]);

		static int run(InitValues const &initVals, std::function<void()> init);
		static int run(std::function<void()> init);
		static void restart();

	private:
		static bool restartFlag;

		static int eventThreadRun(InitValues const &initVals, std::function<void()> init);
		static int monoThreadrun(InitValues const &initVals, std::function<void()> init);
	};
}
