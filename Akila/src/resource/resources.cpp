#include "akila/resource/resources.hpp"

using namespace akila;

std::unordered_map<TypeName, std::unique_ptr<IResourceMap>> Resources::maps;
