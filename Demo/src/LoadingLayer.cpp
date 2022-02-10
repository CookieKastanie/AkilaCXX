#include "Demo/LoadingLayer.hpp"

#include <Akila/files/ResourceFileLoader.hpp>
#include "Demo/GameLayer.hpp"

LoadingLayer::LoadingLayer(): Akila::Layer{} {
	Akila::ResourceFileLoader::fillResourcePool(Akila::Core::resourcePool.get(), "resources.json", [&]() {
		LOG("WOW")
		Akila::Core::layerManager->add(new GameLayer{});
		Akila::Core::layerManager->remove(this);
	});

	shader = Akila::Core::resourcePool->shaders.get("loadingAnimation");
	triangle = Akila::Core::resourcePool->meshs.get("");
}

void LoadingLayer::update() {

}

void LoadingLayer::draw() {
	Akila::Core::renderer->useDefaultFrameBuffer();

	Akila::Core::renderer->disable(Akila::Renderer::Capability::DEPTH_TEST);
	Akila::Core::renderer->disable(Akila::Renderer::Capability::CULL_FACE);

	shader->bind();
	//shader->send("color", {1.f, 0.f, 1.f});
	triangle->draw();
}
