#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <akila/akila.hpp>
#include "test_layer.hpp"

using namespace akila;

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Signals::registerType<int>(Signals::Stack::FRAME_START);

	Listener l;

	//*/
	{
	l = Signals::listen<int>([](int const &e) {
		std::cout << "read " << e << std::endl;
		if(e > 0) Signals::emit<int>(e - 1);
	});

	Listener l2 = Signals::listen<int>([](int const &e) {
		std::cout << "aaa " << e << std::endl;
	});

	Signals::emit<int>(5);

	Signals::flush(Signals::Stack::FRAME_START);
	Signals::flush(Signals::Stack::FRAME_START);
	
	}
	Signals::emit<int>(2);
	Signals::flush(Signals::Stack::FRAME_START);
	Signals::flush(Signals::Stack::FRAME_START);

	Signals::flush(Signals::Stack::FRAME_START);

	Signals::flush(Signals::Stack::FRAME_START);
	Signals::flush(Signals::Stack::FRAME_START);

	//*/


	/*/
	return Core::run([]() {
		std::cout << "Running" << std::endl;
		Window::setTitle("Akila !");
		Layers::add<TestLayer>();
	});
	//*/
}
