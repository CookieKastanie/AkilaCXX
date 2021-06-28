#include "Akila/core/Core.hpp"
#include "Akila/core/Time.hpp"
#include <iostream>

using namespace Akila;

std::shared_ptr<Display> Core::display = nullptr;
std::shared_ptr<StateManager> Core::stateManager = nullptr;
std::shared_ptr<TaskManager> Core::taskManager = nullptr;
std::shared_ptr<Renderer> Core::renderer = nullptr;

Core::Core() {}

int Core::run(int argc, char *argv[], void (*init)(void)) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 0);

	FileSystem::init();

	std::cout << FileSystem::path("resources/textures/citron.png") << std::endl;
	std::cout << FileSystem::exist("resources/textures/citron.png") << std::endl;

	display = std::make_shared<Display>();
	stateManager = std::make_shared<StateManager>();
	taskManager = std::make_shared<TaskManager>();
	renderer = std::make_shared<Renderer>(display);

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

	return EXIT_SUCCESS;
}
