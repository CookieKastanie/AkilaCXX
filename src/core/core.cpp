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
#include <string>

#define CXXOPTS_NO_EXCEPTIONS
#include <cxxopts.hpp>

using namespace akila;
using namespace akila::internal;

bool Core::restartFlag = false;

Core::InitValues Core::parseArguments(int argc, char *argv[]) {
	InitValues values;
	parseArguments(values, argc, argv);
	return values;
}

void Core::parseArguments(InitValues &initValues, int argc, char *argv[]) {
	std::string exeName = "Akila";
	if(argc > 0) {
		exeName = std::string{argv[0]};
	}

	cxxopts::Options options{exeName, R"---(                   
 /\  |  . |  _
/--\ |< | | (_|

Some of these options can voluntarily be ignored by the application !
)---"};

	options
		.allow_unrecognised_options()
		.add_options()
		("wSizeX", "Window default X size", cxxopts::value<int>())
		("wSizeY", "Window default Y size", cxxopts::value<int>())
		("wPosX", "Window default X position", cxxopts::value<int>())
		("wPosY", "Window default Y position", cxxopts::value<int>())
		("wTitle", "Window title", cxxopts::value<std::string>())
		("wDeco", "Enable / disable window decoration", cxxopts::value<bool>())
		("wVisib", "Window visibility at start", cxxopts::value<bool>())
		("wVSync", "Enable / disable vSync", cxxopts::value<bool>())
		("wSamples", "Samples count of backbuffer", cxxopts::value<int>())

		("fsPath", "Define root of the internal filesystem", cxxopts::value<std::string>())

		("gamepadAsJoystick", "Gamepads will be considered joysticks", cxxopts::value<bool>())

		("maxThreadCount", "Max amount of additional working thread", cxxopts::value<unsigned int>())

		("t,useSystemEventsThread", "Handle system events in a thread", cxxopts::value<bool>())
		
		("h,help", "Print help");

	cxxopts::ParseResult result = options.parse(argc, argv);

	///

	if(result.count("wSizeX"))
		initValues.window.size.x = result["wSizeX"].as<int>();

	if(result.count("wSizeY"))
		initValues.window.size.y = result["wSizeY"].as<int>();

	if(result.count("wPosX") || result.count("wPosY"))
		initValues.window.position = IVec2{0, 0};

	if(result.count("wPosX"))
		initValues.window.position.x = result["wPosX"].as<int>();

	if(result.count("wPosY"))
		initValues.window.position.y = result["wPosY"].as<int>();

	if(result.count("wTitle"))
		initValues.window.title = result["wTitle"].as<std::string>();

	if(result.count("wDeco"))
		initValues.window.decoaration = result["wDeco"].as<bool>();

	if(result.count("wVisib"))
		initValues.window.visible = result["wVisib"].as<bool>();

	if(result.count("wVSync"))
		initValues.window.vSync = result["wVSync"].as<bool>();

	if(result.count("wSamples"))
		initValues.window.samples = result["wSamples"].as<int>();

	///

	if(result.count("fsPath"))
		initValues.fileSystem.useAbsolutePath = result["fsPath"].as<std::string>();

	///

	if(result.count("gamepadAsJoystick"))
		initValues.inputs.gamepadAsJoystick = result["gamepadAsJoystick"].as<bool>();

	///

	if(result.count("maxThreadCount"))
		initValues.threadpool.maxThreadCount = result["maxThreadCount"].as<unsigned int>();

	///

	if(result.count("useSystemEventThread"))
		initValues.useSystemEventsThread = result["useSystemEventsThread"].as<bool>();

	///

	if(result.count("help")) {
		std::cout << options.help() << std::endl;
		std::exit(0);
	}
}

int Core::run(InitValues const &initVals, std::function<void()> init) {
	if(initVals.useSystemEventsThread) {
		return eventThreadRun(initVals, std::move(init));
	} else {
		return monoThreadrun(initVals, std::move(init));
	}
}

// TODO : duplicated code

int Core::eventThreadRun(InitValues const &initVals, std::function<void()> init) {
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

int Core::monoThreadrun(InitValues const &initVals, std::function<void()> init) {
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
