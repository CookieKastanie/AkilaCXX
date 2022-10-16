#include "kitchen_layer.hpp"
#include "kitchen.hpp"
#include <akila/default/systems.hpp>
#include <akila/default/components.hpp>

using namespace akila;

void KitchenLayer::onMount() {
	ECS::createSystem<OrbitCameraSystem>();
	ECS::createEntity(ECS::createSignature<OrbitCameraComponent>());
	renderSystem = ECS::createSystem<RenderSystem>();

	Kitchen::loadShaders();

	Resources::load("kitchen.json", []() {
		Kitchen::bakeCubemapFromEqui(Resources::get<Texture2D>("desert"));
		//Kitchen::bakeCubemapFromEqui(Resources::get<Texture2D>("desert"), {2048, 2048});
	});

	Resources::set<StaticMesh>("invertedCube", StaticMeshPrimitives::invertedCube());
}

void KitchenLayer::onUnmount() {

}

void KitchenLayer::tick() {

}

void KitchenLayer::frame() {
	ECS::getSystem<OrbitCameraSystem>()->update();
	renderSystem->colorPass(&ECS::getSystem<OrbitCameraSystem>()->getMainCam().getComponent<OrbitCameraComponent>());





	auto shader = Resources::get<Shader>("skybox");
	auto skybox = Resources::get<TextureCubmap>("bake_render_target");
	auto invertedCube = Resources::get<StaticMesh>("invertedCube");

	Renderer::disable(Renderer::Capability::DEPTH_TEST);
	shader->bind();
	skybox->bind();
	invertedCube->draw();
}

void KitchenLayer::gui() {

}
