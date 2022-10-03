#include "rat_layer.hpp"

//#include <akila/default/resources/static_mesh_primitives.hpp>
#include <akila/default/systems.hpp>
#include <akila/default/components.hpp>



using namespace akila;
/*
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

		c.resize(Window::getSize());
		c.recalcProjection();

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
*/

void RatLayer::onMount() {
	Entity e = ECS::createEntity();
	e.addComponent<MeshComponent>({
		Resources::get<StaticMesh>("rat"),
		Resources::get<Material>("rat")->copy()
		//{Resources::get<Material>("rat")}
	});
	e.addComponent<TransformComponent>();

	
	ECS::createSystem<OrbitCameraSystem>();
	ECS::createEntity(ECS::createSignature<OrbitCameraComponent>());

	Renderer::setClearColor(.3f, .3f, .3f);
	renderSystem = ECS::createSystem<RenderSystem>();

	//music = Resources::get<AudioSource>("rat");
	//music->setVolume(.5);
	//music->setLooping(true);
	//music->play();
}

void RatLayer::onUnmount() {
	ECS::eraseSystem<RenderSystem>();
	ECS::resetAll();
	Resources::cleanAll();
}

void RatLayer::tick() {

}

void RatLayer::frame() {
	ECS::getSystem<OrbitCameraSystem>()->update();

	renderSystem->colorPass(&ECS::getSystem<OrbitCameraSystem>()->getMainCam().getComponent<OrbitCameraComponent>());
}

void RatLayer::gui() {

}
