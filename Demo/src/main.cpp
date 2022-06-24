#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <akila/akila.hpp>
#include <akila/default/loaders.hpp>
#include <akila/default/layers.hpp>

#include "test_layer.hpp"

using namespace akila;

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	return Core::run([]() {
		FileSystem::setRootFolder("resources");
		Window::setTitle("Akila !");

		Resources::registerLoader<MaterialLoader>();

		Layers::add<TestLayer>();
		Layers::add<DebugLayer>();

		Window::setSize({1920, 1080});
		Window::setPosition({100, 100});
	});
}
