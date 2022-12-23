#include "akila/core/layer/layers.hpp"

using namespace akila;

std::vector<std::unique_ptr<Layer>> Layers::layers;
std::vector<Layer*> Layers::mounts;
std::vector<TypeId> Layers::unmounts;

std::vector<std::unique_ptr<Layer>> const &Layers::listing() {
	return layers;
}

void Layers::init() {

}

void Layers::terminate() {
	layers.clear();
	mounts.clear();
	unmounts.clear();
}

void Layers::updateMounts() {
	while(!mounts.empty()) {
		Layer *layer = mounts.back();
		mounts.pop_back();

		for(auto it = layers.begin(); it != layers.end(); ++it) {
			if(it->get()->depth > layer->depth) {
				layers.emplace(it, layer);
				layer->onMount();
				return;
			}
		}

		layers.emplace_back(layer);
		layer->onMount();
	}
}

void Layers::updateUnmounts() {
	while(!unmounts.empty()) {
		TypeId typeId = unmounts.back();
		unmounts.pop_back();

		for(auto it = layers.begin(); it != layers.end(); ++it) {
			if(it->get()->typeId == typeId) {
				it->get()->onUnmount();
				layers.erase(it);
				break;
			}
		}
	}
}

void Layers::tick() {
	for(auto const &layer : layers) layer->tick();
}

void Layers::frame() {
	for(auto const &layer : layers) layer->frame();
}

void Layers::gui() {
	for(auto const &layer : layers) layer->gui();
}

void Layers::removeAll() {
	layers.clear();
}
