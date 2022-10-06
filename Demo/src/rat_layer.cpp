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

struct RatComponent {
	Vec3 rotationSpeeds;
	Vec3 vel;
};

class RatBehaviourSystem: public System {
public:
	float dt = 0;

	RatBehaviourSystem(): System(ECS::createSignature<RatComponent, TransformComponent>()) {}

	void update() {
		dt -= Time::fixedDelta;

		if(dt <= 0) {
			dt = 0.1;
			//*/
			Entity e = ECS::createEntity();
			e.addComponent<MeshComponent>({
				Resources::get<StaticMesh>("rat"),
				Resources::get<Material>("rat")->copy()
				//{Resources::get<Material>("rat")}
			});
			e.addComponent<TransformComponent>();
			e.addComponent<RatComponent>({
				{Random::getAngle(), Random::getAngle(), Random::getAngle()},
				{Random::getFloat(), Random::getFloat(), Random::getFloat()}
			});
			//*/
		}


		for(Entity e : entities) {
			auto &transform = e.getComponent<TransformComponent>();
			auto &rat = e.getComponent<RatComponent>();

			transform.savePrevious();

			transform.rotateX(rat.rotationSpeeds.x * Time::fixedDelta);
			transform.rotateY(rat.rotationSpeeds.y * Time::fixedDelta);
			transform.rotateZ(rat.rotationSpeeds.z * Time::fixedDelta);

			rat.vel.y -= Time::fixedDelta;

			transform.translate(rat.vel * Time::fixedDelta);

			if(transform.position.y < -10) {
				ECS::addToEraseQueue(e);
			}
		}

		ECS::flushEraseQueue();
	}
};

void RatLayer::onMount() {
	{
		Entity e = ECS::createEntity();
		e.addComponent<MeshComponent>({
			Resources::get<StaticMesh>("rat"),
			Resources::get<Material>("rat")->copy()
			//{Resources::get<Material>("rat")}
			});
		e.addComponent<TransformComponent>();
		e.addComponent<RatComponent>({
			{Random::getAngle(), Random::getAngle(), Random::getAngle()},
			{Random::getFloat(), Random::getFloat(), Random::getFloat()}
		});
	}

	{
		Entity e = ECS::createEntity();
		e.addComponent<MeshComponent>({
			Resources::get<StaticMesh>("rat"),
			Resources::get<Material>("rat")->copy()
			//{Resources::get<Material>("rat")}
			});
		e.addComponent<TransformComponent>();
		//e.getComponent<TransformComponent>().translate({2, 0, 0});
		//e.getComponent<TransformComponent>().savePrevious();
	}
	
	ECS::createSystem<RatBehaviourSystem>();

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
	ECS::getSystem<RatBehaviourSystem>()->update();
}

void RatLayer::frame() {
	ECS::getSystem<OrbitCameraSystem>()->update();

	renderSystem->colorPass(&ECS::getSystem<OrbitCameraSystem>()->getMainCam().getComponent<OrbitCameraComponent>());
}

void RatLayer::gui() {

}
