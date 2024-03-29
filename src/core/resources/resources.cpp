#include "akila/core/resources/resources.hpp"

using namespace akila;

std::unordered_map<TypeId, std::unique_ptr<internal::IResourceMap>> Resources::maps;
std::unordered_map<TypeId, std::string> Resources::mapNames;

std::vector<std::unique_ptr<Loader>> Resources::loaders;
std::vector<internal::LoadingInstance> Resources::loadingInstances;
