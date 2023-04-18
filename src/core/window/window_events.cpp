#include "akila/core/window/window_events.hpp"
#include "akila/core/signals/signals.hpp"
#include "akila/core/window/imgui_handler.hpp"
#include "akila/core/window/window.hpp"
#include <iostream>

using namespace akila::internal;

std::vector<akila::WindowResizeSignal> WindowEvents::windowResizeSignals;
std::vector<akila::KeyboardSignal> WindowEvents::keyboardSignals;
std::vector<akila::MouseButtonSignal> WindowEvents::mouseButtonSignals;
std::vector<akila::ControllerSignal> WindowEvents::controllerSignals;

WindowEvents::MouseData WindowEvents::accumulatedMouse;
WindowEvents::MouseData WindowEvents::frameMouse;
WindowEvents::MouseData WindowEvents::interpoledMouse;

std::mutex WindowEvents::mux;

void WindowEvents::init() {
	Inputs::init();

	akila::Signals::registerType<WindowResizeSignal>(akila::Signals::Stack::BEFORE_FRAME);
	akila::Signals::registerType<KeyboardSignal>(akila::Signals::Stack::BEFORE_TICK);
	akila::Signals::registerType<MouseButtonSignal>(akila::Signals::Stack::BEFORE_TICK);
	akila::Signals::registerType<ControllerSignal>(akila::Signals::Stack::BEFORE_TICK);
}

void WindowEvents::resizeCallback(GLFWwindow *window, int width, int height) {
	(void)window;

	std::scoped_lock<std::mutex> lock(mux);

	IVec2 size;
	size.x = max(width, 1);
	size.y = max(height, 1);

	windowResizeSignals.push_back({size});
}

void WindowEvents::terminate() {
	Inputs::terminate();
}

void WindowEvents::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	(void)window;
	(void)scancode;
	(void)mods;

	std::scoped_lock<std::mutex> lock(mux);

	switch(action) {
		case GLFW_PRESS:
			keyboardSignals.push_back({
				static_cast<Keyboard::Key>(key),
				KeyboardSignal::Action::PRESS
			});
			break;

		case GLFW_RELEASE:
			keyboardSignals.push_back({
				static_cast<Keyboard::Key>(key),
				KeyboardSignal::Action::RELEASE
			});
			break;

		case GLFW_REPEAT:
			break;
	}
}

void WindowEvents::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
	(void)window;
	(void)mods;

	if(ImGuiHandler::wantCaptureMouse()) return;

	std::scoped_lock<std::mutex> lock(mux);

	switch(action) {
		case GLFW_PRESS:
			mouseButtonSignals.push_back({
				static_cast<Mouse::Button>(button),
				MouseButtonSignal::Action::PRESS
			});
			break;

		case GLFW_RELEASE:
			mouseButtonSignals.push_back({
				static_cast<Mouse::Button>(button),
				MouseButtonSignal::Action::RELEASE
			});
			break;
	}
}

void WindowEvents::cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
	(void)window;

	if(ImGuiHandler::wantCaptureMouse()) return;

	std::scoped_lock<std::mutex> lock(mux);
	accumulatedMouse.lastPosition = accumulatedMouse.position;

	accumulatedMouse.position.x = static_cast<float>(xpos);
	accumulatedMouse.position.y = static_cast<float>(ypos);

	accumulatedMouse.velocity += accumulatedMouse.position - accumulatedMouse.lastPosition;
}

void WindowEvents::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
	(void)window;

	if(ImGuiHandler::wantCaptureMouse()) return;

	std::scoped_lock<std::mutex> lock(mux);
	accumulatedMouse.scrollVelocity.x += static_cast<float>(xoffset);
	accumulatedMouse.scrollVelocity.y += static_cast<float>(yoffset);
}

void WindowEvents::joystickCallback(int jid, int e) {
	std::scoped_lock<std::mutex> lock(mux);

	//ControllerSignal signal;
	//bool recycled = false;

	switch(e) {
		case GLFW_CONNECTED:
			Inputs::addController(jid);

			//recycled = Inputs::addController(jid);

			//signal.joystick = Inputs::getJoysticks().at(jid).createReference();

			//if(recycled) signal.state = ControllerSignal::State::RECONNECTED;
			//else signal.state = ControllerSignal::State::CONNECTED;

			//controllerSignals.push_back(signal);

			//??????

			break;

		case GLFW_DISCONNECTED:
			//auto it = Inputs::getJoysticks().find(jid);

			//if(it != Inputs::getJoysticks().end()) {
			//	signal.state = JoystickSignal::State::DISCONNECTED;
			//	joystickChange.push_back(signal);
			//	Inputs::removeJoystick(jid);
			//}

			// ????

			Inputs::removeController(jid);

			break;
	}
}

void WindowEvents::process(unsigned int updateCount) {
	std::scoped_lock<std::mutex> lock(mux);

	// calculs mouse
	frameMouse.lastPosition = frameMouse.position;
	frameMouse.position = accumulatedMouse.position;
	frameMouse.velocity = accumulatedMouse.velocity;

	frameMouse.lastScrollPosition = frameMouse.position;
	frameMouse.scrollPosition += accumulatedMouse.scrollVelocity;
	frameMouse.scrollVelocity = accumulatedMouse.scrollVelocity;

	if(updateCount > 0) {
		float const delta = 1.f / updateCount;

		interpoledMouse.lastPosition = interpoledMouse.position;
		interpoledMouse.velocity = (frameMouse.position - interpoledMouse.lastPosition) * delta;

		interpoledMouse.lastScrollPosition = interpoledMouse.scrollPosition;
		interpoledMouse.scrollVelocity = (frameMouse.scrollPosition - interpoledMouse.lastScrollPosition) * delta;
	}

	accumulatedMouse.resetVels();

	// envoi des signaux
	for(WindowResizeSignal const &signal : windowResizeSignals) {
		akila::Signals::emit<WindowResizeSignal>(signal);
	}

	windowResizeSignals.clear();

	for(KeyboardSignal const &signal : keyboardSignals) {
		Inputs::setKeyboardKey(signal.key, static_cast<bool>(signal.action));
		akila::Signals::emit<KeyboardSignal>(signal);
	}

	keyboardSignals.clear();

	for(MouseButtonSignal const &signal : mouseButtonSignals) {
		Inputs::setMouseButton(signal.key, static_cast<bool>(signal.action));
		akila::Signals::emit<MouseButtonSignal>(signal);
	}

	mouseButtonSignals.clear();

	for(ControllerSignal const &signal : controllerSignals) {
		akila::Signals::emit<ControllerSignal>(signal);
	}

	controllerSignals.clear();

	Inputs::updateControllers();
}

void WindowEvents::beforeTick() {
	interpoledMouse.position += interpoledMouse.velocity;
	interpoledMouse.scrollPosition += interpoledMouse.scrollVelocity;

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
