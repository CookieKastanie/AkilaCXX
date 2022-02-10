#include "Demo/GameLayer.hpp"
#include "Demo/MouseCamera.hpp"

GameLayer::GameLayer() {
	sword = Akila::Core::resourcePool->meshs.get("sword");
	swordMaterial = Akila::Core::resourcePool->materials.get("sword");

	Akila::Core::renderer->setSharedCamera(std::make_shared<MouseCamera>(Akila::Core::display->getMouse()));
	Akila::Core::renderer->setClearColor(0, 0, 0);
}

void GameLayer::update() {

}

void GameLayer::draw() {
	Akila::Core::renderer->useDefaultFrameBuffer();

	Akila::Core::renderer->depthFunc(Akila::Renderer::DepthFunc::LESS);
	Akila::Core::renderer->disable(Akila::Renderer::Capability::CULL_FACE);
	Akila::Core::renderer->enable(Akila::Renderer::Capability::DEPTH_TEST);
	Akila::Core::renderer->clear();

	Akila::Core::renderer->render(swordMaterial.raw(), sword.raw());

	/*/
	Akila::ResourceReference<Akila::Shader> shader = Akila::Core::resourcePool->shaders.get("textureTest");
	Akila::ResourceReference<Akila::Texture> texture = Akila::Core::resourcePool->textures.get("env");
	Akila::ResourceReference<Akila::Mesh> mesh = Akila::Core::resourcePool->meshs.get("");

	shader->bind();
	shader->send("tex", 1);
	texture->bind(1);
	mesh->draw();
	//*/
}
