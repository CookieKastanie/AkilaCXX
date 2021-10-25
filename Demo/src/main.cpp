#include "Akila/core/Core.hpp"
#include "Demo/LoadingLayer.hpp"

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

/*

void *operator new(std::size_t size, const char *file, int line) {
	printf("new -> %s : %d\n", file, line);
	return std::malloc(size);
}

#define new new(__FILE__, __LINE__)
//*/

#include "Akila/files/ResourceFile.hpp"

int main(int argc, char *argv[]) {
	return Akila::Core::run(argc, argv, [](void) {
		Akila::FileSystem::setResourceFolder("resources"); // path relative to the executable
		Akila::Core::layerManager->add(new LoadingLayer{});
		//Akila::Core::display->setFullscreen(true);

		Akila::ResourceFile res;
		res.unserializeFrom("main.res");
	});
}
