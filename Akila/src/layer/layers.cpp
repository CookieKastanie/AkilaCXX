#include "akila/layer/layers.hpp"

using namespace akila;

std::vector<std::unique_ptr<Layer>> Layers::layers;
std::vector<Layer*> Layers::mounts;
std::vector<TypeId> Layers::unmounts;
