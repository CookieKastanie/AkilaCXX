#include "akila/core/core.hpp"
#include "akila/window/window.hpp"
#include "akila/layer/layers.hpp"
#include "akila/time/time.hpp"
#include "akila/signal/signals.hpp"
#include <thread>

using namespace akila;

int Core::run(void (*init)(void)) {
	Window::initWindow();

	volatile bool stop = false;
	volatile bool threadFinished = false;
	std::thread thread{[&]() {
		Window::iniGraphicContext();
		
		init();

		Time::update();
		float accumulator = 0;
		while(!stop) {
			Time::update();

			Signals::flush(Signals::Stack::FRAME_START);

			accumulator += Time::delta;

			while(accumulator >= Time::fixedDelta) {
				Signals::flush(Signals::Stack::BEFORE_UPDATE);
				Layers::update();
				accumulator -= Time::fixedDelta;
			}

			Time::mix = accumulator / Time::fixedDelta;

			Signals::flush(Signals::Stack::BEFORE_DRAW);
			Layers::draw();
			//Layers::drawImGui();
			Window::swapBuffers();
		}

		threadFinished = true;
		glfwPostEmptyEvent();
	}};

	while(!Window::shouldClose()) {glfwWaitEvents();}
	stop = true;

	Layers::removeAll();

	glfwPostEmptyEvent();
	while(!threadFinished) glfwWaitEvents();
	thread.join();

	Window::terminate();

	return EXIT_SUCCESS;
}
