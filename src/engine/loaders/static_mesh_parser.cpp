#include "akila/engine/loaders/static_mesh_loader.hpp"
#include "akila/engine/loaders/gltf_parser.hpp"
#include "akila/core/memory/ptr.hpp"
#include "akila/core/resources/file_system.hpp"
#include "akila/core/resources/resources.hpp"
#include "akila/engine/graphics/static_mesh.hpp"
#include "akila/core/threadpool/threadpool.hpp"

using namespace akila;

StaticMeshLoader::StaticMeshLoader(): Loader{"static_mesh"} {}

void StaticMeshLoader::onEntry(JSON json, LoaderCallback cb) {
	if(json["path"].is_string() == false) {
		cb.fail();
		return;
	}

	if(json["name"].is_string() == false && json["prefix"].is_string() == false) {
		cb.fail();
		return;
	}

	Ptr<GlTFParser> parser = createPtr<GlTFParser>();
	Ptr<bool> parserSuccess = createPtr<bool>(false);
	
	Threadpool::submit([=]() {
		if(json["invertTexcoordsY"].is_boolean()) {
			bool invert = json["invertTexcoordsY"];
			parser->setInvertTexcoords(invert);
		}

		std::string path = json["path"];
		*parserSuccess = parser->loadFile(FileSystem::path(path));

		}, [=]() {
			if(*parserSuccess == false) {
				cb.fail();
				return;
			}

			bool useName = json["name"].is_string();

			std::vector<GlTF> meshes = parser->getResult();
			for(GlTF const &mesh : meshes) {
				Ref<StaticMesh> resource;
				if(useName) {
					resource = Resources::create<StaticMesh>(json["name"]);
				} else {
					std::string prefix = json["prefix"];
					resource = Resources::create<StaticMesh>(prefix + mesh.name);
				}
				
				for(auto const &[attribute, buffer] : mesh.attributes) {
					auto vbo = createPtr<VBO>(buffer.componentTypeCount, attribute);
					vbo->setDataType(buffer.componentType);
					vbo->setRawData(buffer.data, buffer.byteCount, 0, buffer.count);
					resource->addVBO(vbo);
				}
				
				if(mesh.indices.byteCount != 0) {
					GlTF::Buffer const &buffer = mesh.indices;
					auto ibo = createPtr<IBO>();
					ibo->setDataType(buffer.componentType);
					ibo->setRawData(buffer.data, buffer.byteCount, 0, buffer.count);
					resource->setIBO(ibo);
				}

				resource->prepare();
			}

			cb.success();
		}
	);
}
