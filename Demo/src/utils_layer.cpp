#include "utils_layer.hpp"
#include "test_layer.hpp"

using namespace akila;

UtilsLayer::UtilsLayer() {
	keyListener = Signals::listen<KeyPressSignal>([](KeyPressSignal const &keySignal) {
		switch(keySignal.key) {
			case Inputs::Key::ESC:
				Window::close();
				break;

			case Inputs::Key::TAB:
				Window::setFullscreen(!Window::isFullscreen());
				break;
		}
	});

	Resources::load({"main.json"}, []() {
		std::cout << "Loaded" << std::endl;
		Layers::add<TestLayer>();
	});
}

void UtilsLayer::tick() {}
void UtilsLayer::frame() {}
void UtilsLayer::gui() {}
