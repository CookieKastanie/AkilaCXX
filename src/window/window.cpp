#include "akila/window/window.hpp"
#include "akila/window/window_events.hpp"
#include "akila/window/imgui_handler.hpp"
#include <iostream>

using namespace akila;

GLFWwindow *Window::window = nullptr;
bool Window::vSync = false;

IVec2 Window::sizeBeforeFS = {100, 100};
IVec2 Window::positionBeforeFS = {0, 0};

bool Window::mouseGrab = false;

Window::InitValues::InitValues():
	size{640, 480},
	title{""},
	visible{true},
	vSync{true},
	samples{0} {
}

void Window::initWindow(InitValues const &initVals) {
	if(!glfwInit())
		std::exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, initVals.samples);
	glfwWindowHint(GLFW_VISIBLE, initVals.visible ? GLFW_TRUE : GLFW_FALSE);

	window = glfwCreateWindow(initVals.size.x, initVals.size.y, initVals.title.c_str(), NULL, NULL);
	if(!window) {
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	glfwSetWindowSizeCallback(window, internal::WindowEvents::resizeCallback);
	glfwSetKeyCallback(window, internal::WindowEvents::keyCallback);
	glfwSetMouseButtonCallback(window, internal::WindowEvents::mouseButtonCallback);
	glfwSetCursorPosCallback(window, internal::WindowEvents::cursorPosCallback);
	glfwSetScrollCallback(window, internal::WindowEvents::scrollCallback);
	
	setVerticalSync(initVals.vSync);

	internal::WindowEvents::init();
}

void Window::initGraphicContext() {
	glfwMakeContextCurrent(window);
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to retrieve OpenGL functions" << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

void Window::setSize(IVec2 const &size) {
	glfwSetWindowSize(window, size.x, size.y);
}

IVec2 Window::getSize() {
	IVec2 size;
	glfwGetWindowSize(window, &size.x, &size.y);
	return max(size, {1, 1});
}

void Window::setPosition(IVec2 const &pos) {
	glfwSetWindowPos(window, pos.x, pos.y);
}

void Window::setPositionToCenter() {
	GLFWmonitor *primary = glfwGetPrimaryMonitor();

	int x, y, w, h;
	glfwGetMonitorWorkarea(primary, &x, &y, &w, &h);

	IVec2 size = getSize();
	setPosition({w / 2 - size.x / 2, h / 2 - size.y / 2});
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

bool Window::isMouseGrabbed() {
	return mouseGrab;
}

void Window::setMouseGrab(bool grab) {
	if(mouseGrab == grab) return;

	glfwSetInputMode(
		window,
		GLFW_CURSOR,
		grab ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL
	);
	
	mouseGrab = grab;
}

bool Window::isVisible() {
	return glfwGetWindowAttrib(window, GLFW_VISIBLE) != 0;
}

void Window::setVisibility(bool visible) {
	if(visible) glfwShowWindow(window);
	else glfwHideWindow(window);
}

bool Window::isDecoarated() {
	return glfwGetWindowAttrib(window, GLFW_DECORATED) != 0;
}

void Window::setDecoaration(bool decoration) {
	glfwSetWindowAttrib(window, GLFW_DECORATED, decoration);
}

float Window::getDPI() {
	float xscale, yscale;
	glfwGetWindowContentScale(window, &xscale, &yscale);
	return max(xscale, yscale);
}

void Window::loadImGuiFont(std::string const &path) {
	internal::ImGuiHandler::loadFont(path, getDPI());
}

void Window::terminate() {
	glfwDestroyWindow(window);
	glfwTerminate();
}
