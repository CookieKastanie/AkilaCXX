#include "akila/engine/loaders/material_loader.hpp"
#include "akila/engine/graphics/material_factory.hpp"
#include "akila/core/resources/file_system.hpp"
#include <fstream>

using namespace akila;

MaterialLoader::MaterialLoader(): Loader{"material"} {}

void MaterialLoader::onEntry(JSON json, LoaderCallback cb) {
	if(json["name"].is_string() == false) {
		cb.fail();
		return;
	}

	if(
		json["source_path"].is_string() == false &&
		json["source_name"].is_string() == false
	) {
		cb.fail();
		return;
	}

	Ref<Material> mat;
	std::string name = json["name"];

	if(json["source_name"].is_string()) {
		mat = MaterialFactory::build(name, json["source_name"]);
	} else {
		std::string path = json["source_path"];
		std::ifstream file;
		file.open(FileSystem::path(path).c_str());
		if(file.good() == false) {
			std::cerr << "Source loading error : can't read " << path << std::endl;
			cb.fail();
			return;
		}

		std::stringstream stream;
		stream << file.rdbuf();

		MaterialFactory::setSource(name, stream.str());
		mat = MaterialFactory::build(name, name);
	}





	cb.success();

	/*/

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
	//*/
}
