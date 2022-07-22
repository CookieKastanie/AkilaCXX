#include "test_layer.hpp"

#include <akila/default/resources/static_mesh_primitives.hpp>

using namespace akila;

GLuint vb;

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

struct EditorData {
	bool hasPLayer = false;
	bool hasPosition = false;
	bool hasRectangle = false;
};

class EditorSystem: public System {
public:
	EditorSystem(): System{ECS::createSignature<>()} {};

	void onAdd(Entity e) override {
		EditorData ed;

		ed.hasPLayer = e.hasComponent<Player>();
		ed.hasPosition = e.hasComponent<Position>();
		ed.hasRectangle = e.hasComponent<Rectangle>();

		e.addComponent<EditorData>(ed);
	}

	void renderUI() {
		ImGui::Begin("Entities");

		for(Entity e : entities) {
			std::string name = "Entity " + std::to_string(e);
			ImGui::Text(name.c_str());

			auto& ed = e.getComponent<EditorData>();

			bool hasPlayer = e.hasComponent<Player>();
			std::string cbLabel = "Player###" + std::to_string(e);
			ImGui::Checkbox(cbLabel.c_str(), &ed.hasPLayer);
			if(ed.hasPLayer != hasPlayer) {
				if(ed.hasPLayer) e.addComponent<Player>();
				else e.removeComponent<Player>();
			}
			/*
			bool hasPosition = e.hasComponent<Position>();
			if(hasPosition) {
				ImGui::Text("Is a player");
			}
			*/

			ImGui::Separator();
		}

		ImGui::End();
	}
};

class PositionSystem : public System {
public:
	PositionSystem(): System{ECS::createSignature<Position>()} {};

	void update() {
		for(Entity e : entities) {
			Position& pos = e.getComponent<Position>();
			pos.old = pos.current;
		}
	}
};

class RenderRectangleSystem: public System {
public:
	RenderRectangleSystem(): System{ECS::createSignature<Rectangle, Position>()} {};

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
	PlayerSystem(): System{ECS::createSignature<Player, Position>()} {};

	void update() {
		for(Entity e : entities) {
			Position &pos = e.getComponent<Position>();

			if(Inputs::isPressed(Inputs::Key::RIGHT)) {
				pos.current.x += 1000 * Time::fixedDelta;
			}

			if(Inputs::isPressed(Inputs::Key::LEFT)) {
				pos.current.x -= 1000 * Time::fixedDelta;
			}

			if(Inputs::isPressed(Inputs::Key::UP)) {
				pos.current.y += 1000 * Time::fixedDelta;
			}

			if(Inputs::isPressed(Inputs::Key::DOWN)) {
				pos.current.y -= 1000 * Time::fixedDelta;
			}
		}
	}

	void renderImGui() {
		if(entities.empty()) return;

		Entity e = *entities.begin();
		Position &pos = e.getComponent<Position>();
			
		ImGui::Begin("Test");
		ImGui::SliderFloat("Value", &pos.current.x, 0, 1000);
		ImGui::End();

		ImGui::Begin("Test2");
		ImGui::InputFloat("Same Value", &pos.current.x);
		ImGui::End();
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
	ECS::createSystem<EditorSystem>();
	ECS::createSystem<PositionSystem>();
	ECS::createSystem<PlayerSystem>();
	ECS::createSystem<RenderRectangleSystem>();


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

	///////

	keyListener = Signals::listen<KeyPressSignal>([](KeyPressSignal const &keySignal) {
		switch(keySignal.key) {
			case Inputs::Key::ESC:
				Window::close();
				break;

			case Inputs::Key::TAB:
				Window::setFullscreen(!Window::isFullscreen());
				break;
		}
	});

	///////

	Resources::registerLoader<LoaderTest>();

	Renderer::disable(Renderer::Capability::DEPTH_TEST);
	Renderer::disable(Renderer::Capability::CULL_FACE);
	Renderer::enable(Renderer::Capability::SCISSOR_TEST);

	Resources::load({"a.json", "b.json"}, []() {
		std::cout << "Loaded" << std::endl;
	});
	

	simpleMat = Resources::get<Material>("simple");
	simpleMat->use("blue");

	//auto cube = Resources::set<StaticMesh>("unitCube", SaticMeshPrimitives::cube());

	glGenVertexArrays(1, &vb);
}

void TestLayer::update() {
	ECS::getSystem<PositionSystem>()->update();
	ECS::getSystem<PlayerSystem>()->update();
}

void TestLayer::draw() {
	Renderer::useDefaultFrameBuffer();

	Renderer::disable(Renderer::Capability::DEPTH_TEST);
	Renderer::disable(Renderer::Capability::CULL_FACE);
	Renderer::disable(Renderer::Capability::SCISSOR_TEST);
	Renderer::setClearColor(.5f, .2f, .8f);
	Renderer::clearColor();


	float t = sin(Time::now) * .5 + .5;
	simpleMat->write("blue", t);
	simpleMat->send();

	glBindVertexArray(vb);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);


	Renderer::enable(Renderer::Capability::SCISSOR_TEST);
	ECS::getSystem<RenderRectangleSystem>()->render();

	//IVec2 p{sin(Time::now * 4.f) * 420.f + (1600 / 3), 100};
	//Window::setPosition(p);

	IVec2 p = Window::getPosition();
	Window::setTitle(to_string(p));
}

void TestLayer::drawImGui() {
	ECS::getSystem<PlayerSystem>()->renderImGui();
	ECS::getSystem<EditorSystem>()->renderUI();

	ImGui::Begin("Texture");
	ImGui::Image((ImTextureID)Resources::get<Texture2D>("link")->getId(), {460, 460}, {0, 1}, {1, 0});
	ImGui::End();
}
