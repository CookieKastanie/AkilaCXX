#include "akila/engine/loaders/shader_source_loader.hpp"
#include "akila/engine/graphics/material_factory.hpp"
#include "akila/core/resources/file_system.hpp"
#include <fstream>

using namespace akila;

ShaderSourceLoader::ShaderSourceLoader(): Loader{"shader_source"} {}

void ShaderSourceLoader::onEntry(JSON json, LoaderCallback cb) {
	if(
		json["name"].is_string() == false ||
		json["path"].is_string() == false
	) {
		cb.fail();
		return;
	}

	std::string name = json["name"];
	std::string path = json["path"];

	std::ifstream file;
	file.open(FileSystem::resources(path).c_str());
	if(file.good() == false) {
		std::cerr << "Source loading error : can't read " << FileSystem::resources(path) << std::endl;
		cb.fail();
		return;
	}

	std::stringstream stream;
	stream << file.rdbuf();

	MaterialFactory::setSource(name, stream.str());

	cb.success();
}
