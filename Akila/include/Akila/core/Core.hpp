#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Akila/core/Display.hpp"
#include "Akila/core/Layer.hpp"
#include "Akila/core/Coroutine.hpp"
#include "Akila/core/Time.hpp"
#include "Akila/graphics/Renderer.hpp"
#include "Akila/files/FileSystem.hpp"
#include "Akila/core/ResourcePool.hpp"

#include "Akila/core/Memory.hpp"
#include <iostream>

#ifdef IMGUI
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#endif

#define LOG(x) std::cout << x << std::endl;

namespace Akila {
	class Core {
		private:
			Core();

		public:
			static Ptr<Display> display;
			static Ptr<LayerManager> layerManager;
			static Ptr<CoroutineManager> coroutines;
			static Ptr<Renderer> renderer;
			static Ptr<ResourcePool> resourcePool;

			static int run(int argc, char *argv[], void (*init)(void));
	};
}
