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

int Core::run(InitValues const &initVals, std::function<void()> init) {
	if(initVals.systemEventSingleThread) {
		return monoThreadRun(initVals, std::move(init));
	} else {
		return defaultRun(initVals, std::move(init));
	}
}

// TODO : duplicated code

int Core::defaultRun(InitValues const &initVals, std::function<void()> init) {
start:
	restartFlag = false;

	Threadpool::init(initVals.threadpool);
	Window::initWindow(initVals.window);
	Inputs::init(initVals.inputs);
	Audio::init();
	FileSystem::init(initVals.fileSystem);
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
		Time::init();

		threadReady = true;

		init();

		ImGuiHandler::initIniFileName();

		while(!stop) {
			Time::update();

			WindowEvents::process(Time::tickCountThisFrame());

			Threadpool::flush();

			Layers::updateUnmounts();
			Layers::updateMounts();

			Time::beforeTicksLoop();
			while(Time::tickTimeRemaining()) {
				Time::beforeTick();
				WindowEvents::beforeTick();
				Signals::flush(Signals::Stack::BEFORE_TICK);
				Layers::tick();
			}

			Time::calculateMix();
			Time::beforeFrame();

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
	Inputs::terminate();
	Window::terminate();

	if(restartFlag) goto start;

	return EXIT_SUCCESS;
}

int Core::monoThreadRun(InitValues const &initVals, std::function<void()> init) {
start:
	restartFlag = false;

	Threadpool::init(initVals.threadpool);
	Window::initWindow(initVals.window);
	Inputs::init(initVals.inputs);
	Audio::init();
	FileSystem::init(initVals.fileSystem);
	Signals::init();
	Resources::init();
	ECS::init();
	Layers::init();

	Window::initGraphicContext();
	Renderer::init();
	ImGuiHandler::init();
	Time::init();

	init();

	ImGuiHandler::initIniFileName();

	while(Window::shouldClose() == false) {
		glfwPollEvents();

		Time::update();

		WindowEvents::process(Time::tickCountThisFrame());

		Threadpool::flush();

		Layers::updateUnmounts();
		Layers::updateMounts();

		Time::beforeTicksLoop();
		while(Time::tickTimeRemaining()) {
			Time::beforeTick();
			WindowEvents::beforeTick();
			Signals::flush(Signals::Stack::BEFORE_TICK);
			Layers::tick();
		}

		Time::calculateMix();
		Time::beforeFrame();

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

	ECS::terminate();
	Layers::terminate();
	Resources::terminate();
	Signals::terminate();
	Threadpool::terminate();
	ImGuiHandler::terminate();
	Audio::terminate();
	Inputs::terminate();
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
