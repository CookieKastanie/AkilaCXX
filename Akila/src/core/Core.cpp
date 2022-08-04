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
#include "akila/resource/resources.hpp"
#include "akila/renderer/renderer.hpp"
#include "akila/random/random.hpp"
#include "akila/audio/audio.hpp"

#include <thread>
#include <atomic>

using namespace akila;
using namespace akila::internal;

int Core::run(void (*init)(void)) {
	Random::init();
	Window::initWindow();
	Audio::init();
	FileSystem::init();
	Threadpool::init();

	std::atomic<bool> stop = false;
	std::atomic<bool> threadReady = false;
	std::atomic<bool> threadFinished = false;
	std::thread thread{[&]() {
		Window::initGraphicContext();
		Renderer::init();
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
				WindowEvents::beforeTick();
				Signals::flush(Signals::Stack::BEFORE_TICK);
				Layers::tick();
				accumulator -= Time::fixedDelta;
			}

			Time::mix = accumulator / Time::fixedDelta;

			WindowEvents::beforeFrame();
			Signals::flush(Signals::Stack::BEFORE_FRAME);
			Layers::frame();

			ImGuiHandler::beginFrame();
			Layers::gui();
			ImGuiHandler::endFrame();

			Window::swapBuffers();
		}

		Layers::removeAll();
		ECS::resetAll();
		Resources::cleanAll();

		threadFinished = true;
		glfwPostEmptyEvent();
	}};

	while(!threadReady); // attente active mais c'est bon ca va hein me faites pas chier ca dure moins de 1ms

	while(!Window::shouldClose()) {glfwWaitEvents();}
	stop = true;

	glfwPostEmptyEvent();
	while(!threadFinished) glfwWaitEvents();
	thread.join();

	Threadpool::terminate();
	ImGuiHandler::terminate();
	Audio::terminate();
	Window::terminate();

	return EXIT_SUCCESS;
}
