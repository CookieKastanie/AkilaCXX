#include "rat_layer.hpp"

#include <akila/default/resources/static_mesh_primitives.hpp>
#include <akila/default/systems.hpp>
#include <akila/default/components.hpp>

using namespace akila;

struct MeshComponent {
	Ref<StaticMesh> mesh;
	Ptr<Material> material;
};

class DrawRatSystem: public System {
public:
	DrawRatSystem(): System{ECS::createSignature<TransformComponent, MeshComponent>()} {
		shader = Resources::get<Shader>("texLit");
		camSystem = ECS::getSystem<OrbitCameraSystem>();
	}

	void draw() {
		Entity cam = camSystem->getMainCam();
		auto &c = cam.getComponent<OrbitCameraComponent>();

		//shader->bind();
		shader->send("PV", c.pv);

		for(Entity e : entities) {
			auto &oc = e.getComponent<MeshComponent>();
			auto &transform = e.getComponent<TransformComponent>();

			transform.rotateY(Time::delta * PI * 0.5f);
			transform.setScale(15.f);
			transform.position.y = -3.f;

			shader->send("model", transform.calcMatrix());
			oc.material->send();
			oc.mesh->draw();
		}
	}

private:
	Ref<Shader> shader;
	OrbitCameraSystem *camSystem;
};

RatLayer::RatLayer(): Layer{} {
	Entity e = ECS::createEntity();
	e.addComponent<MeshComponent>({
		Resources::get<StaticMesh>("rat"),
		Resources::get<Material>("rat")->copy()
	});
	e.addComponent<TransformComponent>();

	Renderer::setClearColor(.3f, .3f, .3f);
	Renderer::enable(Renderer::Capability::DEPTH_TEST);

	ECS::createSystem<OrbitCameraSystem>();
	ECS::createEntity(ECS::createSignature<OrbitCameraComponent>());

	ECS::createSystem<DrawRatSystem>();

	music = Resources::get<AudioSource>("rat");
	music->setVolume(.5);
	music->setLooping(true);
	music->play();
}

void RatLayer::tick() {

}

void RatLayer::frame() {
	ECS::getSystem<OrbitCameraSystem>()->update();

	Renderer::useDefaultFrameBuffer();
	Renderer::clear();
	ECS::getSystem<DrawRatSystem>()->draw();
}

void RatLayer::gui() {

}
