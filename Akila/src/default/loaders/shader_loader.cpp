#include "akila/default/loaders/shader_loader.hpp"
#include "akila/akila.hpp"
#include "akila/default/loaders/uniform_parser.hpp"

using namespace akila;

ShaderLoader::ShaderLoader(): Loader{"shader"} {}

void ShaderLoader::onEntry(JSON json, LoaderCallback cb) {
	if(!json["name"].is_string()) {
		cb.fail();
		return;
	}

	std::string name = json["name"];

	if(!json["path"].is_string()) {
		cb.fail();
		return;
	}

	std::string shaderPath = json["path"];
	std::ifstream file;
	file.open(FileSystem::path(shaderPath).c_str());
	if(!file.good()) {
		std::cerr << "Shader loading error : can't read " << shaderPath << std::endl;
		cb.fail();
		return;
	}

	std::stringstream stream;
	stream << file.rdbuf();

	auto &shader = Resources::create<Shader>(name, stream.str());

	Parser::parseUniforms(shader, json, [&](std::string const &name, UniformInfos const &infos, void *data, std::size_t byteCount) {
		(void)(name);
		shader->sendRaw(infos, data);
	});

	cb.success();
}
