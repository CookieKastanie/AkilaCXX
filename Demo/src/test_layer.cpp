#include "test_layer.hpp"

#include <akila/default/resources/static_mesh_primitives.hpp>
#include <akila/default/systems.hpp>
#include <akila/default/components.hpp>

using namespace akila;

/*/
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

			ImGui::Separator();
		}

		ImGui::End();
	}
};
//*/

/*/
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
//*/
/*
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
};//*/

struct MeshComponent {
	Ref<StaticMesh> mesh;
	Ptr<Material> material;
};

class DrawSystem: public System {
public:
	DrawSystem(): System{ECS::createSignature<TransformComponent, MeshComponent>()} {
		//shader = Resources::get<Shader>("wireframe");
		shader = Resources::get<Shader>("pbr_flat");
		camSystem = ECS::getSystem<OrbitCameraSystem>();
	}

	void draw() {
		Entity cam = camSystem->getMainCam();
		auto& c = cam.getComponent<OrbitCameraComponent>();
		
		//shader->send("PV", c.pv);

		shader->send("u_camera.view", c.view);
		shader->send("u_camera.projection", c.projection);

		for(Entity e : entities) {
			auto &oc = e.getComponent<MeshComponent>();
			auto &transform = e.getComponent<TransformComponent>();

			//shader->send("model", transform.calcMatrixMix());
			shader->send("modelMatrix", transform.calcMatrixMix());
			oc.material->send();
			oc.mesh->draw();
		}
	}

private:
	Ref<Shader> shader;
	OrbitCameraSystem *camSystem;
};

//>

struct PlayerComponent {
	int id;
};

class PlayerSystem: public System {
public:
	PlayerSystem(): System{ECS::createSignature<PlayerComponent, TransformComponent>()} {};

	void update() {
		for(Entity e : entities) {
			auto &transform = e.getComponent<TransformComponent>();
			transform.savePrevious();

			Vec3 mov{};

			if(Inputs::isPressed(Inputs::Key::RIGHT)) {
				mov.x += 10 * Time::fixedDelta;
			}

			if(Inputs::isPressed(Inputs::Key::LEFT)) {
				mov.x -= 10 * Time::fixedDelta;
			}

			if(Inputs::isPressed(Inputs::Key::UP)) {
				mov.z -= 10 * Time::fixedDelta;
			}

			if(Inputs::isPressed(Inputs::Key::DOWN)) {
				mov.z += 10 * Time::fixedDelta;
			}

			transform.translate(mov);
		}
	}
};


class AudioSystem: public System {
public:
	AudioSystem(): System{ECS::createSignature<TransformComponent, AudioEmitter>()} {}

	void play() {
		for(Entity e : entities) {
			auto &t = e.getComponent<TransformComponent>();
			auto &s = e.getComponent<AudioEmitter>();
			s.setPosition(t.position);
			Audio::detach(s);
			//s.play();

			std::cout << t.position << std::endl;
		}
	}
};

//
#include <chrono>
TestLayer::TestLayer(): Layer{} {
	ECS::createSystem<OrbitCameraSystem>();
	ECS::createEntity(ECS::createSignature<OrbitCameraComponent>());

	ECS::createSystem<DrawSystem>();
	ECS::createSystem<PlayerSystem>();


	//auto unitCubeMesh = Resources::set<StaticMesh>("unitCube", StaticMeshPrimitives::cube());
	auto unitCubeMesh = Resources::get<StaticMesh>("smooth_cube");
	auto wireframeMat = Resources::get<Material>("wireframe");

	for(int i = 0; i < 3; ++i) {
		Entity e = ECS::createEntity(ECS::createSignature<MeshComponent, TransformComponent>());
		if(i == 0) e.addComponent<PlayerComponent>();

		auto &mc = e.getComponent<MeshComponent>();
		auto &transform = e.getComponent<TransformComponent>();

		mc.mesh = unitCubeMesh;
		mc.material = wireframeMat->copy();

		Vec3 color{0, 0, 0};
		color[i] = 1;
		mc.material->write("color", color);
		transform.position = color * 2.f;
		transform.position.y += .5;
		transform.savePrevious();

		if(i == 0) {
			e.addComponent<AudioEmitter>(AudioEmitter{Resources::get<AudioSource>("boom")});
		}
	}

	for(int i = 0; i < 20; ++i) {
		Entity e = ECS::createEntity(ECS::createSignature<MeshComponent, TransformComponent>());

		auto &mc = e.getComponent<MeshComponent>();
		auto &transform = e.getComponent<TransformComponent>();

		mc.mesh = Resources::get<StaticMesh>("palmtree");
		mc.material = wireframeMat->copy();

		Vec3 color{.5f, .5f, .5f};
		mc.material->write("color", color);
		//transform.position = {4, 0, 0};

		Vec2 pos = Random::getVec2(-10, 10);
		transform.position = {pos.x, 0, pos.y};
		//transform.rotateY(PI / 2.f);
		transform.rotateY(Random::getAngle());
		transform.setScale(Random::getFloat(.7f, 1.3f));

		transform.savePrevious();
	}

	//*/
	{
		Entity e = ECS::createEntity(ECS::createSignature<MeshComponent, TransformComponent>());

		auto &mc = e.getComponent<MeshComponent>();
		auto &transform = e.getComponent<TransformComponent>();

		mc.mesh = Resources::set<StaticMesh>("plane", StaticMeshPrimitives::quad());
		mc.material = wireframeMat->copy();

		Vec3 color{.1f, .8f, .1f};
		mc.material->write("color", color);
		transform.setScale(11);
		transform.rotateX(-PI / 2.f);

		transform.savePrevious();
	}
	//*/

	Renderer::setClearColor(.5f, .2f, .8f);
	Renderer::enable(Renderer::Capability::DEPTH_TEST);
	



	music = Resources::get<AudioSource>("hound");
	//boom.setSource(Resources::get<AudioSource>("boom"));

	ECS::createSystem<AudioSystem>();
	
	keyListener = Signals::listen<KeyPressSignal>([&](KeyPressSignal const &e) {
		if(e.key == Inputs::Key::P) {
			music->play();
		}
		

		if(e.key == Inputs::Key::B) {
			ECS::getSystem<AudioSystem>()->play();
		}
		/*
		if(e.key == Inputs::Key::B) {
			auto &t = entity.getComponent<TransformComponent>();

			boom.setPosition(t.position);

			Audio::detach(boom);
		}*/
	});
}

void TestLayer::tick() {
	//ECS::getSystem<PositionSystem>()->update();
	ECS::getSystem<PlayerSystem>()->update();
}

void TestLayer::frame() {
	//for(auto it = emitters.begin(); it != emitters.end();) {
	//	if(it->isFinished()) it = emitters.erase(it);
	//	else ++it;
	//}

	//std::cout << emitters.size() << std::endl;

	ECS::getSystem<OrbitCameraSystem>()->update();

	Entity e = ECS::getSystem<OrbitCameraSystem>()->getMainCam();
	auto &c = e.getComponent<OrbitCameraComponent>();

	Audio::setListenerPositionDirection(c.position, normalize(c.center - c.position));

	Renderer::useDefaultFrameBuffer();
	Renderer::clear();
	ECS::getSystem<DrawSystem>()->draw();
	

	/*/
	//Renderer::disable(Renderer::Capability::DEPTH_TEST);
	Renderer::disable(Renderer::Capability::CULL_FACE);
	Renderer::disable(Renderer::Capability::SCISSOR_TEST);
	Renderer::setClearColor(.5f, .2f, .8f);
	Renderer::clear();

	Entity cam = ECS::getSystem<OrbitCameraSystem>()->getMainCam();
	auto &camData = cam.getComponent<OrbitCameraComponent>();

	camData.resize(Window::getSize());

	auto shader = simpleMat->getShaderRef();
	shader->bind();
	shader->send("PV", camData.pv);


	float t = sin(Time::now) * .5 + .5;
	simpleMat->write("hue", t);
	simpleMat->send();

	glBindVertexArray(vb);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	auto &unlitShader = Resources::get<Shader>("unlit");
	unlitShader->bind();
	unlitShader->send("PV", camData.pv);
	auto& cube = Resources::get<StaticMesh>("unitCube");

	unlitShader->send("color", Vec3{1, 0, 0});
	cube->draw();

	unlitShader->send("color", Vec3{0, 1, 0});
	cube->draw();

	unlitShader->send("color", Vec3{0, 0, 1});
	cube->draw();




	Renderer::enable(Renderer::Capability::SCISSOR_TEST);
	ECS::getSystem<RenderRectangleSystem>()->render();
	//*/
}

void TestLayer::gui() {
	//ECS::getSystem<PlayerSystem>()->renderImGui();
	//ECS::getSystem<EditorSystem>()->renderUI();
}
