#include "akila/core/core.hpp"
#include "akila/window/window.hpp"
#include "akila/window/window_events.hpp"
#include "akila/layer/layers.hpp"
#include "akila/time/time.hpp"
#include "akila/signal/signals.hpp"
#include "akila/ecs/ecs.hpp"
#include "akila/window/imgui_handler.hpp"
#include "akila/threadpool/threadpool.hpp"
#include "akila/resource/file_system.hpp"
#include <thread>
#include <atomic>

using namespace akila;
using namespace akila::internal;

int Core::run(void (*init)(void)) {
	Window::initWindow();
	FileSystem::init();
	Threadpool::init();

	std::atomic<bool> stop = false;
	std::atomic<bool> threadReady = false;
	std::atomic<bool> threadFinished = false;
	std::thread thread{[&]() {
		Window::initGraphicContext();
		ImGuiHandler::init();

		threadReady = true;

		init();

		Time::update();
		float accumulator = 0;
		while(!stop) {
			Time::update();
			accumulator += Time::delta;

			WindowEvents::process(accumulator / Time::fixedDelta);

			Threadpool::flush();

			while(accumulator >= Time::fixedDelta) {
				WindowEvents::beforeUpdate();
				Signals::flush(Signals::Stack::BEFORE_UPDATE);
				Layers::update();
				accumulator -= Time::fixedDelta;
			}

			Time::mix = accumulator / Time::fixedDelta;

			WindowEvents::beforeDraw();
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

	while(!threadReady); // attente active mais c'est bon ca va hein me faites pas chier ca dure moins de 1ms

	while(!Window::shouldClose()) {glfwWaitEvents();}
	stop = true;

	Layers::removeAll();
	ECS::resetAll();

	glfwPostEmptyEvent();
	while(!threadFinished) glfwWaitEvents();
	thread.join();

	Threadpool::terminate();
	ImGuiHandler::terminate();
	Window::terminate();

	return EXIT_SUCCESS;
}
