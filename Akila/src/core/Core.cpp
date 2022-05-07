#include "akila/core/core.hpp"
#include "akila/window/window.hpp"
#include "akila/layer/layers.hpp"
#include <iostream>

#include <glm/gtx/string_cast.hpp>
using namespace akila;

void test() {
	IVec2 s = Window::getSize();
	glViewport(0, 0, s.x, s.y);

	glDisable(GL_SCISSOR_TEST);
	glClearColor(.5f, .2f, .8f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 0, 100, 100);
	long t = (long)(glfwGetTime() * 255);
	glClearColor((float)(t % 255) / 255.f, .5f, .2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	IVec2 p = Window::getPosition();
	Window::setTitle(glm::to_string(p));
}

#include <thread>

int Core::run(void (*init)(void)) {
	Window::initWindow();

	volatile bool stop = false;
	volatile bool threadFinished = false;
	std::thread thread{[&]() {
		Window::iniGraphicContext();
		init();
		while(!stop) {
			test();
			Layers::update();
			Layers::draw();
			Layers::drawImGui();
			Window::swapBuffers();
		}

		threadFinished = true;
		glfwPostEmptyEvent();
	}};

	while(!Window::shouldClose()) {glfwWaitEvents();}
	stop = true;

	glfwPostEmptyEvent();
	while(!threadFinished) glfwWaitEvents();
	thread.join();

	Window::terminate();

	return EXIT_SUCCESS;
}
