#include "akila/resource/resources.hpp"

using namespace akila;

std::unordered_map<TypeId, std::unique_ptr<IResourceMap>> Resources::maps;
