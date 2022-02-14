#include "Akila/core/Layer.hpp"

using namespace Akila;

Layer::Layer(): index{-1}, depth{PreferredDepth::MIDDLE} {}

Layer::~Layer() {}

void Layer::update() {}

void Layer::draw() {}

//#ifdef IMGUI
void Layer::drawImGui() {}
//#endif

///////////////////////////////////////////////////////////////////////////////

LayerManager::LayerManager() {}

void LayerManager::add(const Ptr<Layer> &layer, Layer::PreferredDepth depth) {
	if(layer->index != -1) {
		std::cerr << "Layer is already assigned to a manager" << std::endl;
		return;
	}

	layer->depth = depth;

	bool haveInserted = false;
	auto it = layers.begin();
	while(it != layers.end()) {
		if(it->get()->depth > depth) {
			layer->index = it - layers.begin();
			it = layers.insert(it, layer) + 1;
			haveInserted = true;
			break;
		} else {
			++it;
		}
	}

	while(it != layers.end()) {
		++it->get()->index;
		++it;
	}

	if(!haveInserted) {
		layer->index = layers.size();
		layers.push_back(layer);
	}
}

void LayerManager::add(Layer *layer, Layer::PreferredDepth depth) {
	add(createPtr(layer), depth);
}

void LayerManager::remove(Layer *layer) {
	if(layer->index >= 0 && layer->index < layers.size()) {
		const int index = layers[layer->index]->index;
		layers[layer->index]->index = -1;
		layers.erase(layers.begin() + index);
	}
}

void LayerManager::update() {
	for(const auto &layer : layers) layer->update();
}

void LayerManager::draw() {
	for(const auto &layer : layers) layer->draw();
}

#ifdef IMGUI
void LayerManager::drawImGui() {
	for(const auto &layer : layers) layer->drawImGui();
}
#endif
