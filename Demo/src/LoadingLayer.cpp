#include "Demo/LoadingLayer.hpp"

#include <Akila/files/ResourceFileLoader.hpp>

LoadingLayer::LoadingLayer(): Akila::Layer{} {
	Akila::ResourceFileLoader::fillResourcePool(Akila::Core::resourcePool.get(), "resources.json", []() {});
}

void LoadingLayer::update() {

}

void LoadingLayer::draw() {
	Akila::Core::renderer->useDefaultFrameBuffer();
}
