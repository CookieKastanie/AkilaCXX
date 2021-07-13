#include "Akila/core/Display.hpp"
#include <iostream>

using namespace Akila;

Display::Display() {
	window = glfwCreateWindow(1, 1, "", NULL, NULL);

	if(window == NULL) {
		glfwTerminate();
		std::cerr << "Failed to create window" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	makeCurrent();

	setSize(600, 600);
	setLimits(128, 64, DONT_CARE, DONT_CARE);

	setVerticalSync(true);

	glfwSetWindowUserPointer(window, this);

	glfwSetWindowSizeCallback(window, [](GLFWwindow *window, int width, int height) -> void {
		Display *self = (Display*)glfwGetWindowUserPointer(window);
		self->width = width;
		self->height = height;

		if(self->width <= 0) self->width = 1;
		if(self->height <= 0) self->height = 1;

		self->rendererResizeCallback();
	});

	glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) -> void {
		Display *self = (Display *)glfwGetWindowUserPointer(window);

		if(key != GLFW_KEY_UNKNOWN) {
			const char *name = glfwGetKeyName(key, 0);
			if(name != NULL) {
				int n = *name;
				if(n >= 'a' && n <= 'z') n -= 0x20; // lowercase to uppercase
				key = n;
			}
			
			self->keybord.setKeyState((Keyboard::Key)key, action != GLFW_RELEASE);

			if(action == GLFW_PRESS) self->keybord.firePressEvent();
		}
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int button, int action, int mods) -> void {
		Display *self = (Display *)glfwGetWindowUserPointer(window);
		self->mouse.setKeyState((Mouse::Key)button, action != GLFW_RELEASE);

		if(action == GLFW_PRESS) self->mouse.firePressEvent();
	});

	glfwSetCursorPosCallback(window, [](GLFWwindow *window, double xpos, double ypos) ->void {
		Display *self = (Display *)glfwGetWindowUserPointer(window);
		self->mouse.setPosition((float)xpos, (float)ypos);
	});
}

void Display::setSize(int w, int h) {
	glfwSetWindowSize(window, w, h);
	width = w;
	height = h;
}

int Display::getWidth() {
	return width;
}

int Display::getHeight() {
	return height;
}

void Display::setLimits(int minW, int minH, int maxW, int maxH) {
	glfwSetWindowSizeLimits(window, minW, minH, maxW, maxH);
}

void Display::setTitle(const char *title) {
	glfwSetWindowTitle(window, title);
}

void Display::makeCurrent() {
	glfwMakeContextCurrent(window);
}

void Display::setVerticalSync(bool b) {
	glfwSwapInterval(b);
	vSync = b;
}

bool Display::shouldClose() {
	return glfwWindowShouldClose(window);
}

void Display::swapBuffers() {
	glfwSwapBuffers(window);
}

bool Display::isFullscreen() {
	return glfwGetWindowMonitor(window) != nullptr;
}

void Display::setFullscreen(bool fullscreen) {
	if(isFullscreen() == fullscreen) return;

	if(fullscreen) {
		glfwGetWindowSize(window, &winSize[0], &winSize[1]);
		glfwGetWindowPos(window, &winPos[0], &winPos[1]);

		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode *mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, 0);
	} else {
		glfwSetWindowMonitor(window, nullptr, winPos[0], winPos[1], winSize[0], winSize[1], 0);
	}

	glfwSwapInterval(vSync);
}

Keyboard *Display::getKeybord() {
	return &keybord;
}

Mouse *Display::getMouse() {
	return &mouse;
}

void Display::setRendererResizeCallback(const std::function<void()> &cb) {
	rendererResizeCallback = cb;
}

Display::~Display() {
	glfwDestroyWindow(window);
}
