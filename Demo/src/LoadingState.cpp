#include "Demo/LoadingState.hpp"
#include "Akila/core/Core.hpp"
#include "Akila/core/Time.hpp"
#include <iostream>
#include <string>

LoadingState::LoadingState(): Akila::State{} {

}

void LoadingState::update() {

}

void LoadingState::draw() {
	//std::cout << (1.f / Akila::Time::delta) << std::endl;
	//std::cout << Akila::Core::display->getWidth() << std::endl;

	float fps{1.f / Akila::Time::delta};
	Akila::Core::display->setTitle(std::string("FPS ").append(std::to_string(fps)).c_str());
}
