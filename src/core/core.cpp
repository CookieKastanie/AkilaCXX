#include "akila/core/core.hpp"
#include "akila/core/window/window.hpp"
#include "akila/core/window/window_events.hpp"
#include "akila/core/layer/layers.hpp"
#include "akila/core/time/time.hpp"
#include "akila/core/signals/signals.hpp"
#include "akila/core/ecs/ecs.hpp"
#include "akila/core/window/imgui_handler.hpp"
#include "akila/core/threadpool/threadpool.hpp"
#include "akila/core/resources/file_system.hpp"
#include "akila/core/resources/resources.hpp"
#include "akila/core/rhi/renderer.hpp"
#include "akila/core/audio/audio.hpp"

#include <thread>
#include <atomic>

using namespace akila;
using namespace akila::internal;

bool Core::restartFlag;

int Core::run(Window::InitValues const &initVals, std::function<void()> init) {
start:
	restartFlag = false;

	Threadpool::init();
	Window::initWindow(initVals);
	Audio::init();
	FileSystem::init();
	Signals::init();
	Resources::init();
	ECS::init();
	Layers::init();

	std::atomic<bool> stop = false;
	std::atomic<bool> threadReady = false;
	std::atomic<bool> threadFinished = false;
	std::thread thread{[&]() {
		Window::initGraphicContext();
		Renderer::init();
		ImGuiHandler::init();

		threadReady = true;

		init();

		ImGuiHandler::initIniFileName();

		Time::update();
		float accumulator = 0;
		while(!stop) {
			Time::update();
			accumulator += Time::delta;

			WindowEvents::process(static_cast<unsigned int>(accumulator / Time::fixedDelta));

			Threadpool::flush();

			Layers::updateUnmounts();
			Layers::updateMounts();

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

	while(!threadReady); // attente active

	while(!Window::shouldClose()) {glfwWaitEvents();}
	stop = true;

	glfwPostEmptyEvent();
	while(!threadFinished) glfwWaitEvents();
	thread.join();

	ECS::terminate();
	Layers::terminate();
	Resources::terminate();
	Signals::terminate();
	Threadpool::terminate();
	ImGuiHandler::terminate();
	Audio::terminate();
	Window::terminate();

	if(restartFlag) goto start;

	return EXIT_SUCCESS;
}

int Core::run(std::function<void()> init) {
	return run({}, init);
}

void Core::restart() {
	restartFlag = true;
	Window::close();
}
