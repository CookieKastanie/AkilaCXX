#include "Akila/core/Core.hpp"
#include "Akila/core/Time.hpp"
#include <iostream>

using namespace Akila;

Display *Core::display = nullptr;
TaskManager *Core::taskManager = nullptr;
std::shared_ptr<State> Core::currentState = nullptr;

int Core::run(int argc, char *argv[], void (*init)(void)) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 0);

	display = new Display{};
	taskManager = new TaskManager{};

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to retrieve OpenGL functions" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	init();

	Time::update();
	while(!display->shouldClose()) {
		Time::update();
		taskManager->flush();
		currentState->update();
		currentState->draw();

		display->swapBuffers();
		glfwPollEvents();
	}

	delete taskManager;
	delete display;

	return EXIT_SUCCESS;
}

void Core::setState(std::shared_ptr<State> state) {
	currentState = state;
}

void Core::setState(State *state) {
	setState(std::shared_ptr<State>(state));
}
