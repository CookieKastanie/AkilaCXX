#include "Akila/core/State.hpp"

using namespace Akila;

State::State() {}

State::~State() {}

void State::update() {}

void State::draw() {}

///////////////////////////////////////////////////////////////////////////////

StateManager::StateManager() {
	setState(new State{});
}

void StateManager::setState(std::shared_ptr<State> state) {
	currentState = state;
}

void StateManager::setState(State *state) {
	setState(std::shared_ptr<State>(state));
}

State *StateManager::getCurrentState() {
	return currentState.get();
}
