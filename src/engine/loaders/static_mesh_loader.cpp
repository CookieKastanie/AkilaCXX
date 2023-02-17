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

	if(json["invert_texcoord_y"].is_boolean()) {
		bool invert = json["invert_texcoord_y"];
		parser->setInvertTexcoord(invert);
	}

	if(json["dump_json"].is_boolean()) {
		bool dumpJson = json["dump_json"];
		parser->setDumpJson(dumpJson);
	}

	std::string path = json["path"];
	bool useName = json["name"].is_string();

	std::string name = "";
	std::string prefix = "";
	if(useName) {
		name = json["name"];
	} else {
		prefix = json["prefix"];
	}

	Threadpool::submit([=]() {
		*parserSuccess = parser->loadFile(FileSystem::resources(path));
	}, [=]() {
		if(*parserSuccess == false) {
			cb.fail();
			return;
		}

		std::vector<GlTF> const &meshes = parser->getResult();
		for(GlTF const &mesh : meshes) {
			Ref<StaticMesh> resource;
			if(useName) {
				resource = Resources::create<StaticMesh>(name);
			} else {
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

			Mesh::Bounds bounds;
			bounds.min = mesh.min;
			bounds.max = mesh.max;
			bounds.radius = mesh.radius;
			bounds.squaredRadius = mesh.squaredRadius;
			resource->setBounds(bounds);

			resource->prepare();
		}

		cb.success();
	});
}
