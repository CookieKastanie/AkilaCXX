#include "Akila/core/Core.hpp"
#include "Akila/core/Time.hpp"
#include <iostream>

using namespace Akila;

std::shared_ptr<Display> Core::display = nullptr;
std::shared_ptr<StateManager> Core::stateManager = nullptr;
std::shared_ptr<TaskManager> Core::taskManager = nullptr;
std::shared_ptr<Renderer> Core::renderer = nullptr;
std::shared_ptr<ResourcesBucket> Core::resourcesBucket = nullptr;

Core::Core() {}

int Core::run(int argc, char *argv[], void (*init)(void)) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 0);

	FileSystem::init();
	display = std::make_shared<Display>();

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to retrieve OpenGL functions" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	stateManager = std::make_shared<StateManager>();
	taskManager = std::make_shared<TaskManager>();
	renderer = std::make_shared<Renderer>(display);
	resourcesBucket = std::make_shared<ResourcesBucket>(renderer);

	init();

	Time::update();
	State *currentState = nullptr;
	while(!display->shouldClose()) {
		Time::update();
		taskManager->flush();

		currentState = stateManager->getCurrentState();
		currentState->update();
		renderer->prepare();
		currentState->draw();

		display->swapBuffers();
		glfwPollEvents();
	}

	return EXIT_SUCCESS;
}
