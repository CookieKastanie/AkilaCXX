#include "akila/default/loaders/material_loader.hpp"
#include "akila/akila.hpp"
#include "akila/default/loaders/uniform_parser.hpp"

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

	auto &mat = Resources::create<Material>(name, stream.str());

	Parser::parseUniforms(mat->getShaderRef(), json, [&](std::string const &name, UniformInfos const &infos, void *data, std::size_t byteCount) {
		mat->use(name);
		mat->writeRaw(&infos, data, byteCount);
	});

	Parser::parseTextureBinds(mat->getShaderRef(), json, [&](Ref<TextureBuffer> texRef, int unit) {
		mat->affect(texRef, unit);
	}, [&](Ref<TextureBuffer> texRef, std::string const &unifName) {
		mat->affect(texRef, unifName);
	});

	cb.success();
}
