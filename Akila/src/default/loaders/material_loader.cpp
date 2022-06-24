#include "akila/default/loaders/material_loader.hpp"
#include "akila/akila.hpp"

using namespace akila;

MaterialLoader::MaterialLoader(): Loader{"material"} {}

void MaterialLoader::onEntry(JSON json, LoaderCallback cb) {
	if(!json["name"].is_string()) {
		cb.fail();
		return;
	}

	std::string name = json["name"];

	if(!json["shader"].is_string()) {
		cb.fail();
		return;
	}

	std::string shaderPath = json["shader"];
	std::ifstream file;
	file.open(FileSystem::path(shaderPath).c_str());
	if(!file.good()) {
		std::cerr << "Shader loading error : can't read " << shaderPath << std::endl;
		cb.fail();
		return;
	}

	std::stringstream stream;
	stream << file.rdbuf();

	Resources::create<Material>(name, stream.str());

	cb.success();
}
