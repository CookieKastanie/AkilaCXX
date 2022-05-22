#include "test_layer.hpp"

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

TestLayer::TestLayer(): Layer{} {
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

	a = 2000;

	l = Signals::listen<int>([](int const &e) {
		std::cout << "read " << e << std::endl;
		if(e > 0) Signals::emit<int>(e - 1);
	});

	keyPressListener = Signals::listen<KeyPressSignal>([](KeyPressSignal const s) {
		std::cout << "key press: " << static_cast<int>(s.key) << std::endl;
	});

	x = 0;
	oldX = 0;

	//Time::fixedDelta = 1.f / 10.f;
	Time::fixedDelta = 1.f / 70.f;
}

void TestLayer::update() {
	oldX = x;
	if(--a == 0) {
		//Layers::remove<TestLayer>();
	}

	if(Inputs::isPressed(Inputs::Key::RIGHT)) {
		x += 1000 * Time::fixedDelta;
	}

	if(Inputs::isPressed(Inputs::Key::LEFT)) {
		x -= 1000 * Time::fixedDelta;
	}
}

void TestLayer::draw() {
	IVec2 s = Window::getSize();
	glViewport(0, 0, s.x, s.y);

	glDisable(GL_SCISSOR_TEST);
	glClearColor(.5f, .2f, .8f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_SCISSOR_TEST);
	glScissor((1.f - Time::mix) * oldX + Time::mix * x, 0, 100, 100);
	//glScissor(x, 0, 100, 100);
	long t = (long)(Time::now * 255);
	glClearColor((float)(t % 255) / 255.f, .5f, .2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	//IVec2 p{sin(Time::now * 4.f) * 420.f + (1600 / 3), 100};
	//Window::setPosition(p);

	IVec2 p = Window::getPosition();
	Window::setTitle(to_string(p));
}

void TestLayer::drawImGui() {

}
