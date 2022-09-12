#include "akila/layer/layer.hpp"

using namespace akila;

Layer::Layer(): typeId{0}, depth{Depth::MIDDLE} {}

void Layer::tick() {}
void Layer::frame() {}
void Layer::gui() {}

std::string const Layer::getTypeName() const {
	return typeName;
}
