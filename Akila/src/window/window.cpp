#include "akila/window/window.hpp"
#include "akila/renderer/shader.hpp"
#include <iostream>

using namespace akila;

GLFWwindow *Window::window = nullptr;
bool Window::vSync = false;

IVec2 Window::sizeBeforeFS = {100, 100};
IVec2 Window::positionBeforeFS = {0, 0};

void Window::initWindow() {
    //glfwSetErrorCallback(error_callback);
    
    if(!glfwInit())
        std::exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 480, "", NULL, NULL);
    if(!window) {
        glfwTerminate();
		std::exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, internal::WindowEvents::keyCallback);
	glfwSetMouseButtonCallback(window, internal::WindowEvents::mouseButtonCallback);
	glfwSetCursorPosCallback(window, internal::WindowEvents::cursorPosCallback);
	glfwSetScrollCallback(window, internal::WindowEvents::scrollCallback);
    
	setVerticalSync(true);
	glfwShowWindow(window);

	internal::WindowEvents::init();
}

void Window::initGraphicContext() {
	glfwMakeContextCurrent(window);
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	    std::cerr << "Failed to retrieve OpenGL functions" << std::endl;
	    std::exit(EXIT_FAILURE);
	}

	internal::GL_FUNC_ARRAY::init();
}

void Window::setSize(IVec2 const &size) {
	glfwSetWindowSize(window, size.x, size.y);
}

IVec2 Window::getSize() {
	IVec2 size;
	glfwGetWindowSize(window, &size.x, &size.y);
	return size;
}

void Window::setPosition(IVec2 const &pos) {
	glfwSetWindowPos(window, pos.x, pos.y);
}

IVec2 Window::getPosition() {
	IVec2 pos;
	glfwGetWindowPos(window, &pos.x, &pos.y);
	return pos;
}

void Window::setLimits(IVec2 const &minSize, IVec2 const &maxSize) {
	glfwSetWindowSizeLimits(window, minSize.x, minSize.y, maxSize.x, maxSize.y);
}

void Window::setTitle(std::string const &title) {
	glfwSetWindowTitle(window, title.c_str());
}

void Window::makeCurrent() {
	glfwMakeContextCurrent(window);
}

void Window::setVerticalSync(bool b) {
	glfwSwapInterval(b);
	vSync = b;
}

bool Window::isVerticalSyncEnabled() {
	return vSync;
}

bool Window::shouldClose() {
	return glfwWindowShouldClose(window);
}

void Window::close() {
	glfwSetWindowShouldClose(window, true);
}

void Window::swapBuffers() {
	glfwSwapBuffers(window);
}

bool Window::isFullscreen() {
	return glfwGetWindowMonitor(window) != nullptr;
}

void Window::setFullscreen(bool fullscreen) {
	if(isFullscreen() == fullscreen) return;

	if(fullscreen) {
		sizeBeforeFS = Window::getSize();
		positionBeforeFS = Window::getPosition();

		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		GLFWvidmode const *mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
	} else {
		glfwSetWindowMonitor(
			window, nullptr,
			positionBeforeFS.x, positionBeforeFS.y,
			sizeBeforeFS.x, sizeBeforeFS.y, GLFW_DONT_CARE
		);
	}

	glfwSwapInterval(vSync);
}

void Window::terminate() {
	glfwDestroyWindow(window);
	glfwTerminate();
}
