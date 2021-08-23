#include "Akila/core/Core.hpp"
#include "Demo/LoadingState.hpp"

/*
static int objectCount{0};
static std::size_t memory{0};

void *operator new(std::size_t size) {
	memory += size;
	printf("new : %d objets (%zu octets)\n", ++objectCount, memory);
	size_t *ptr = (size_t *)std::malloc(size + sizeof(std::size_t));
	*ptr = size;
	return ptr + 1;
}

void operator delete(void *basePtr) noexcept {
	size_t *ptr = ((size_t *)basePtr) - 1;
	memory -= *ptr;
	printf("del : %d objets (%zu octets)\n", --objectCount, memory);
	std::free(ptr);
}
//*/

int main(int argc, char *argv[]) {
	return Akila::Core::run(argc, argv, [](void) {
		Akila::FileSystem::setResourceFolder("resources"); // path relative to the executable
		Akila::Core::stateManager->setState(new LoadingState{});
		//Akila::Core::display->setFullscreen(true);
	});
}
