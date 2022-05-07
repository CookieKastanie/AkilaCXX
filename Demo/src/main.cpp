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
	
	Vec3 vec{4, 1.6, -5.3};
	std::cout << vec << std::endl;
	std::cout << vec + Vec3{1, 0, 0} << std::endl;

	std::cout << "=======" << std::endl;

	Signature signature = ECS::createSignature<int>();
	ShowSystem *showSystem = ECS::createSystem<ShowSystem>(signature);

	Entity e0 = ECS::createEntity(ECS::createSignature<int, double>());
	e0.getComponent<double>() = 5.2;
	e0.getComponent<int>() = 11;

	Entity e1 = ECS::createEntity();
	e1.addComponent<int>(7);

	std::cout << "e0 has int " << e0.hasComponent<int>() << std::endl;
	std::cout << "e0 has float " << e0.hasComponent<float>() << std::endl;
	std::cout << "e0 has double " << e0.hasComponent<double>() << std::endl << std::endl;

	std::cout << "e1 has int " << e1.hasComponent<int>() << std::endl;
	std::cout << "e1 has float " << e1.hasComponent<float>() << std::endl;
	std::cout << "e1 has double " << e1.hasComponent<double>() << std::endl << std::endl;

	showSystem->update();

	std::cout << "--------" << std::endl;

	//ECS::eraseEntity(e0);
	ECS::addToEraseQueue(e0);
	ECS::flushEraseQueue();

	showSystem->update();

	std::cout << "=======" << std::endl;

	Ref<float> r = Resources::create<float>("number", 1.2f);

	std::cout << "ref : " << *r << std::endl;
	std::cout << "ref : " << *Resources::get<float>("number") << std::endl;

	*r = 1.8f;
	std::cout << "ref : " << *r << std::endl;

	return Core::run([]() {
		std::cout << "Running" << std::endl;
		Window::setTitle("Akila !");
	});
}
