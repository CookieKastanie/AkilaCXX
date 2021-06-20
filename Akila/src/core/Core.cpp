#include "Akila/core/Core.hpp"
#include "Akila/core/Time.hpp"
#include <iostream>

using namespace Akila;

Display *Core::display = nullptr;
StateManager *Core::stateManager = nullptr;
TaskManager *Core::taskManager = nullptr;

Core::Core() {}

int Core::run(int argc, char *argv[], void (*init)(void)) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 0);

	display = new Display{};
	stateManager = new StateManager{};
	taskManager = new TaskManager{};

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to retrieve OpenGL functions" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	init();

	Time::update();
	State *currentState = nullptr;
	while(!display->shouldClose()) {
		Time::update();
		taskManager->flush();

		currentState = stateManager->getCurrentState();
		currentState->update();
		currentState->draw();

		display->swapBuffers();
		glfwPollEvents();
	}

	delete taskManager;
	delete stateManager;
	delete display;

	return EXIT_SUCCESS;
}
