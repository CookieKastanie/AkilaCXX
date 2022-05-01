#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include <iostream>
#include <akila/akila.hpp>

using namespace akila;

class ShowSystem: public System {
public:
	void update() {
		for(Entity e : entities) {
			int &val = e.getComponent<int>();
			std::cout << val << std::endl;
		}
	}
};

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ECS::registerComponent<double>();
	ECS::registerComponent<int>();

	Signature signature = ECS::createSignature<int>();
	ShowSystem *showSystem = ECS::createSystem<ShowSystem>(signature);

	Entity e0 = ECS::createEntity(ECS::createSignature<int, double>());
	e0.getComponent<double>() = 5.2;
	e0.getComponent<int>() = 11;

	Entity e1 = ECS::createEntity();
	e1.addComponent<int>(7);

	showSystem->update();

	std::cout << "--------" << std::endl;

	//ECS::eraseEntity(e0);
	ECS::addToEraseQueue(e0);
	ECS::flushEraseQueue();

	showSystem->update();

	return 0;
}
