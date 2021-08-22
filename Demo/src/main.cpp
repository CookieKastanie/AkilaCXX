#include "Akila/core/Core.hpp"
#include "Demo/LoadingState.hpp"

int main(int argc, char *argv[]) {
	return Akila::Core::run(argc, argv, [](void) {
		Akila::FileSystem::setResourceFolder("resources"); // path relative to the executable
		Akila::Core::stateManager->setState(new LoadingState{});
		//Akila::Core::display->setFullscreen(true);
	});
}
