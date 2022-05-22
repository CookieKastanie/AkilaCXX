#include "akila/window/window_events.hpp"
#include "akila/signal/signals.hpp"
#include <iostream>

using namespace akila::internal;

std::vector<akila::KeyPressSignal> WindowEvents::keyPressed;
std::vector<akila::KeyReleaseSignal> WindowEvents::keyReleased;

std::mutex WindowEvents::mux;

void WindowEvents::init() {
	Signals::registerType<KeyPressSignal>(Signals::Stack::BEFORE_UPDATE);
	Signals::registerType<KeyReleaseSignal>(Signals::Stack::BEFORE_UPDATE);
}

void WindowEvents::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	std::lock_guard<std::mutex> lock(mux);

	//std::cout << "key: " << key << "(" << static_cast<char>(key) << ")" << " | "
	//	<< scancode << " | " << action << " | " << mods << std::endl;

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

void WindowEvents::emitSignals() {
	std::lock_guard<std::mutex> lock(mux);

	for(KeyPressSignal const &signal : keyPressed) {
		Inputs::setInputState(signal.key, true);
		Signals::emit<KeyPressSignal>(signal);
	}

	keyPressed.clear();

	for(KeyReleaseSignal const &signal : keyReleased) {
		Inputs::setInputState(signal.key, false);
		Signals::emit<KeyReleaseSignal>(signal);
	}

	keyReleased.clear();
}
