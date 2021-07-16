#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Akila/core/Display.hpp"
#include "Akila/core/State.hpp"
#include "Akila/core/Task.hpp"
#include "Akila/core/Time.hpp"
#include "Akila/graphics/Renderer.hpp"
#include "Akila/files/FileSystem.hpp"
#include "Akila/core/ResourcesBucket.hpp"

#include <memory>
#include <iostream>


namespace Akila {
	class Core {
		private:
			Core();

		public:
			static std::shared_ptr<Display> display;
			static std::shared_ptr<StateManager> stateManager;
			static std::shared_ptr<TaskManager> taskManager;
			static std::shared_ptr<Renderer> renderer;
			static std::shared_ptr<ResourcesBucket> resourcesBucket;

			static int run(int argc, char *argv[], void (*init)(void));
	};
}
