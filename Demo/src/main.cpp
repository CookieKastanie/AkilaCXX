#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <akila/akila.hpp>
#include "test_layer.hpp"

using namespace akila;

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Events::registerType<int>(Events::Stack::FRAME_START);

	//*/
	{
	Listener l = Events::listen<int>([](int const &e) {
		std::cout << "read " << e << std::endl;
		if(e > 0) Events::emit<int>(e - 1);
	});

	Events::emit<int>(5);

	Events::flush(Events::Stack::FRAME_START);
	Events::flush(Events::Stack::FRAME_START);
	
	}
	Events::emit<int>(2);
	Events::flush(Events::Stack::FRAME_START);
	Events::flush(Events::Stack::FRAME_START);

	Events::flush(Events::Stack::FRAME_START);

	Events::flush(Events::Stack::FRAME_START);
	Events::flush(Events::Stack::FRAME_START);

	//*/

	/*/
	return Core::run([]() {
		std::cout << "Running" << std::endl;
		Window::setTitle("Akila !");
		Layers::add<TestLayer>();
	});
	//*/
}
