#include "Demo/LoadingLayer.hpp"

#include <Akila/graphics/MeshPrimitives.hpp>
#include "Demo/GameLayer.hpp"

using namespace Akila;

LoadingLayer::LoadingLayer(): Layer{} {
	Core::resourcePool->load("resources.json", [&]() {
		LOG("WOW")
		Core::layerManager->add(new GameLayer{});
		Core::layerManager->remove(this);
	});

	Core::resourcePool->meshs.set("screenTriangle", MeshPrimitives::screenTriangle());

	shader = Core::resourcePool->shaders.get("loadingAnimation");
	triangle = Core::resourcePool->meshs.get("screenTriangle");
}

void LoadingLayer::update() {

}

void LoadingLayer::draw() {
	Core::renderer->useDefaultFrameBuffer();

	Core::renderer->disable(Renderer::Capability::DEPTH_TEST);

	shader->bind();
	//shader->send("color", {1.f, 0.f, 1.f});
	triangle->draw();
}
