#include "akila/core/core.hpp"
#include "akila/window/window.hpp"
#include "akila/window/window_events.hpp"
#include "akila/layer/layers.hpp"
#include "akila/time/time.hpp"
#include "akila/signal/signals.hpp"
#include "akila/ecs/ecs.hpp"
#include "akila/window/imgui_handler.hpp"
#include <thread>

using namespace akila;
using namespace akila::internal;

int Core::run(void (*init)(void)) {
	Window::initWindow();

	volatile bool stop = false;
	volatile bool threadFinished = false;
	std::thread thread{[&]() {
		Window::initGraphicContext();
		ImGuiHandler::init();

		init();

		Time::update();
		float accumulator = 0;
		while(!stop) {
			Time::update();
			accumulator += Time::delta;

			WindowEvents::emitSignals();

			while(accumulator >= Time::fixedDelta) {
				Signals::flush(Signals::Stack::BEFORE_UPDATE);
				Layers::update();
				accumulator -= Time::fixedDelta;
			}

			Time::mix = accumulator / Time::fixedDelta;

			Signals::flush(Signals::Stack::BEFORE_DRAW);
			Layers::draw();

			ImGuiHandler::beginFrame();
			Layers::drawImGui();
			ImGuiHandler::endFrame();

			Window::swapBuffers();
		}

		threadFinished = true;
		glfwPostEmptyEvent();
	}};

	while(!Window::shouldClose()) {glfwWaitEvents();}
	stop = true;

	Layers::removeAll();
	ECS::resetAll();

	glfwPostEmptyEvent();
	while(!threadFinished) glfwWaitEvents();
	thread.join();

	ImGuiHandler::terminate();
	Window::terminate();

	return EXIT_SUCCESS;
}
