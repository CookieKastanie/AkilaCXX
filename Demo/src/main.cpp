#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <akila/akila.hpp>
#include "test_layer.hpp"

using namespace akila;

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//*/
	return Core::run([]() {
		std::cout << "Running" << std::endl;

		Signals::registerType<int>(Signals::Stack::BEFORE_UPDATE);

		Window::setTitle("Akila !");
		Layers::add<TestLayer>();

		Signals::emit<int>(20);
	});
	//*/
}
