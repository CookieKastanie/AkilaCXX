#include "test_layer.hpp"

using namespace akila;

struct Rectangle {
	Vec2 size;
	Vec3 color;
};

struct Position {
	Vec3 current;
	Vec3 old;
};

struct Player {
	int id;
};

class RenderRectangleSystem: public System {
public:
	void render() {
		for(Entity e : entities) {
			Rectangle &rect = e.getComponent<Rectangle>();
			Position &pos = e.getComponent<Position>();

			Vec3 mixedPos = (1.f - Time::mix) * pos.old + Time::mix * pos.current;

			Renderer::scissor(mixedPos.x, mixedPos.y, rect.size.x, rect.size.y);
			Renderer::setClearColor(rect.color.x, rect.color.z, rect.color.z);
			Renderer::clearColor();
		}
	}
};

class PlayerSystem: public System {
public:
	void update() {
		for(Entity e : entities) {
			Position &pos = e.getComponent<Position>();
			pos.old = pos.current;

			if(Inputs::isPressed(Inputs::Key::RIGHT)) {
				pos.current.x += 1000 * Time::fixedDelta;
			}

			if(Inputs::isPressed(Inputs::Key::LEFT)) {
				pos.current.x -= 1000 * Time::fixedDelta;
			}
		}
	}

	void renderImGui() {
		for(Entity e : entities) {
			Position &pos = e.getComponent<Position>();
			
			ImGui::Begin("Test");
			ImGui::SliderFloat("Value", &pos.current.x, 0, 1000);
			ImGui::End();

			ImGui::Begin("Test2");
			ImGui::InputFloat("Same Value", &pos.current.x);
			ImGui::End();
		}
	}
};


class LoaderTest: public Loader {
public:
	LoaderTest(): Loader{"nameA"} {};

	void onEntry(JSON json, LoaderCallback cb) override {
		std::cout << json << std::endl;

		cb.success();
	}
};

TestLayer::TestLayer(): Layer{} {
	ECS::createSystem<PlayerSystem>(ECS::createSignature<Player, Position>());
	ECS::createSystem<RenderRectangleSystem>(ECS::createSignature<Rectangle, Position>());


	Entity e0 = ECS::createEntity(ECS::createSignature<Player, Position, Rectangle>());
	e0.getComponent<Rectangle>().size = {100, 100};
	e0.getComponent<Rectangle>().color = {1, .5, .2};


	Signature rectSign = ECS::createSignature<Position, Rectangle>();
	for(int i = 0; i < 5; ++i) {
		Entity e = ECS::createEntity(rectSign);
		Vec3 pos = {Random::getFloat(0, 600), Random::getFloat(0, 600), 0};
		e.getComponent<Position>().current = pos;
		e.getComponent<Position>().old = pos;
		e.getComponent<Rectangle>().size = {100, 100};
		e.getComponent<Rectangle>().color = {Random::getFloat(), Random::getFloat(), Random::getFloat()};
	}



	l = Signals::listen<int>([](int const &e) {
		std::cout << "read " << e << std::endl;
		if(e > 0) Signals::emit<int>(e - 1);
	});

	keyPressListener = Signals::listen<KeyPressSignal>([](KeyPressSignal const s) {
		std::cout << "key press: " << static_cast<int>(s.key) << std::endl;
	});


	Resources::registerLoader<LoaderTest>();

	Resources::load({"a.json", "b.json"}, []() {
		std::cout << "Loaded" << std::endl;
	});
}

void TestLayer::update() {
	ECS::getSystem<PlayerSystem>()->update();
}

void TestLayer::draw() {
	Renderer::useDefaultFrameBuffer();

	Renderer::disable(Renderer::Capability::SCISSOR_TEST);
	Renderer::setClearColor(.5f, .2f, .8f);
	Renderer::clearColor();

	Renderer::enable(Renderer::Capability::SCISSOR_TEST);
	ECS::getSystem<RenderRectangleSystem>()->render();

	//IVec2 p{sin(Time::now * 4.f) * 420.f + (1600 / 3), 100};
	//Window::setPosition(p);

	IVec2 p = Window::getPosition();
	Window::setTitle(to_string(p));
}

void TestLayer::drawImGui() {
	ECS::getSystem<PlayerSystem>()->renderImGui();
}
