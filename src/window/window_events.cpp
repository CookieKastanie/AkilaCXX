#include "akila/window/window_events.hpp"
#include "akila/signal/signals.hpp"
#include "akila/window/imgui_handler.hpp"
#include "akila/window/window.hpp"
#include <iostream>

using namespace akila::internal;

std::vector<akila::WindowResizeSignal> WindowEvents::resizes;
std::vector<akila::KeyPressSignal> WindowEvents::keyPressed;
std::vector<akila::KeyReleaseSignal> WindowEvents::keyReleased;

WindowEvents::MouseData WindowEvents::accumulatedMouse;
WindowEvents::MouseData WindowEvents::frameMouse;
WindowEvents::MouseData WindowEvents::interpoledMouse;

std::mutex WindowEvents::mux;

void WindowEvents::init() {
	Inputs::init();

	akila::Signals::registerType<WindowResizeSignal>(akila::Signals::Stack::BEFORE_FRAME);
	akila::Signals::registerType<KeyPressSignal>(akila::Signals::Stack::BEFORE_TICK);
	akila::Signals::registerType<KeyReleaseSignal>(akila::Signals::Stack::BEFORE_TICK);
}

void WindowEvents::resizeCallback(GLFWwindow *window, int width, int height) {
	std::scoped_lock<std::mutex> lock(mux);
	resizes.push_back({max(IVec2{width, height}, IVec2{1, 1})});
}

void WindowEvents::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	std::scoped_lock<std::mutex> lock(mux);

	switch(action) {
		case GLFW_PRESS:
			keyPressed.push_back({static_cast<Inputs::Key>(key)});
			break;

		case GLFW_RELEASE:
			keyReleased.push_back({static_cast<Inputs::Key>(key)});
			break;

		case GLFW_REPEAT:
			break;
	}
}

void WindowEvents::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
	if(ImGuiHandler::wantCaptureMouse()) return;

	std::scoped_lock<std::mutex> lock(mux);

	switch(action) {
		case GLFW_PRESS:
			keyPressed.push_back({static_cast<Inputs::Key>(button)});
			break;

		case GLFW_RELEASE:
			keyReleased.push_back({static_cast<Inputs::Key>(button)});
			break;
	}
}

void WindowEvents::cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
	if(ImGuiHandler::wantCaptureMouse()) return;

	std::scoped_lock<std::mutex> lock(mux);
	accumulatedMouse.lastPosition = accumulatedMouse.position;

	accumulatedMouse.position.x = static_cast<float>(xpos);
	accumulatedMouse.position.y = static_cast<float>(ypos);

	accumulatedMouse.velocity += accumulatedMouse.position - accumulatedMouse.lastPosition;
}

void WindowEvents::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
	if(ImGuiHandler::wantCaptureMouse()) return;

	std::scoped_lock<std::mutex> lock(mux);
	accumulatedMouse.scrollVelocity.x += static_cast<float>(xoffset);
	accumulatedMouse.scrollVelocity.y += static_cast<float>(yoffset);
}

void WindowEvents::process(unsigned int updateCount) {
	std::scoped_lock<std::mutex> lock(mux);

	// calculs mouse
	frameMouse.lastPosition = frameMouse.position;
	frameMouse.position = accumulatedMouse.position;
	frameMouse.velocity = accumulatedMouse.velocity;
	frameMouse.scrollVelocity = accumulatedMouse.scrollVelocity;

	float const delta = 1.f / updateCount;
	interpoledMouse.position = frameMouse.lastPosition;
	interpoledMouse.velocity = accumulatedMouse.velocity * delta;
	interpoledMouse.scrollVelocity = accumulatedMouse.scrollVelocity * delta;

	accumulatedMouse.resetVels();

	// envoi des signaux
	for(WindowResizeSignal const &signal : resizes) {
		akila::Signals::emit<WindowResizeSignal>(signal);
	}

	resizes.clear();

	for(KeyPressSignal const &signal : keyPressed) {
		Inputs::setInputState(signal.key, true);
		akila::Signals::emit<KeyPressSignal>(signal);
	}

	keyPressed.clear();

	for(KeyReleaseSignal const &signal : keyReleased) {
		Inputs::setInputState(signal.key, false);
		akila::Signals::emit<KeyReleaseSignal>(signal);
	}

	keyReleased.clear();
}

void WindowEvents::beforeTick() {
	interpoledMouse.position += interpoledMouse.velocity;

	Inputs::setMousePosition(interpoledMouse.position);
	Inputs::setMouseVelocity(interpoledMouse.velocity);
	Inputs::setMouseScrollVelocity(interpoledMouse.scrollVelocity);

	Vec2 const &size = akila::Window::getSize();
	Inputs::setClipSpaceMousePosition({
		(      interpoledMouse.position.x / size.x) * 2.f - 1.f,
		(1.f - interpoledMouse.position.y / size.y) * 2.f - 1.f
	});
}

void WindowEvents::beforeFrame() {
	Inputs::setMousePosition(frameMouse.position);
	Inputs::setMouseVelocity(frameMouse.velocity);
	Inputs::setMouseScrollVelocity(frameMouse.scrollVelocity);

	Vec2 const &size = akila::Window::getSize();
	Inputs::setClipSpaceMousePosition({
		(      frameMouse.position.x / size.x) * 2.f - 1.f,
		(1.f - frameMouse.position.y / size.y) * 2.f - 1.f
	});
}

void WindowEvents::MouseData::resetVels() {
	velocity.x = 0;
	velocity.y = 0;

	scrollVelocity.x = 0;
	scrollVelocity.y = 0;
}
