#include "akila/default/loaders/mesh_loader.hpp"
#include "akila/akila.hpp"
#include "akila/default/components/static_mesh_component.hpp"

using namespace akila;

struct BufferLocator {
	BufferObject::Type componentType = BufferObject::Type::UNSIGNED_SHORT;
	std::size_t byteLength = 0;
	std::size_t byteOffset = 0;
};

void affectBL(BufferLocator &bl, JSON &accessors, JSON &views, std::size_t index) {
	if(index != -1) {
		bl.componentType = accessors[index]["componentType"];
		bl.byteLength = views[index]["byteLength"];
		bl.byteOffset = views[index]["byteOffset"];
	}
};

MeshLoader::MeshLoader(): Loader{"mesh"} {}

void MeshLoader::onEntry(JSON json, LoaderCallback cb) {
	if(!json["name"].is_string() || !json["path"].is_string()) {
		cb.fail();
		return;
	}

	std::string name = json["name"];
	std::string path = json["path"];


	struct _ {
		//std::string const path;

		std::vector<std::uint8_t> raw;

		BufferLocator vertex;
		BufferLocator normal;
		BufferLocator tangent;
		BufferLocator uv0;
		BufferLocator uv1;
		BufferLocator uv2;
		BufferLocator index;
	};

	Ptr<_> p = createPtr<_>();
	//p->path = json["path"];

	StaticMesh *mesh = Resources::create<StaticMesh>(name);

	Threadpool::submit([=]() {
		std::ifstream file(FileSystem::path(path), std::ios::binary);
		if(!file.good()) std::cerr << "Mesh loading error : can't read " << path << std::endl;

		{
			std::uint32_t val = 0;

			file.read(reinterpret_cast<char *>(&val), sizeof(val));
			if(val != 0x46546C67) return; // "glTF"

			file.read(reinterpret_cast<char *>(&val), sizeof(val));
			// verifier la version ?
			//std::cout << "version : " << val << std::endl;

			file.read(reinterpret_cast<char *>(&val), sizeof(val));
			// verifier la longueur a la fin ?
			//std::cout << "length : " << val << std::endl;
		}

		JSON json;
		{ // le premier block doit etre de type JSON
			std::uint32_t chunkLength = 0;
			file.read(reinterpret_cast<char *>(&chunkLength), sizeof(chunkLength));
			std::uint32_t chunkType = 0;
			file.read(reinterpret_cast<char *>(&chunkType), sizeof(chunkType));

			if(chunkType != 0x4E4F534A) return; // "JSON"

			std::string jsonText;
			jsonText.resize(chunkLength);
			file.read(jsonText.data(), chunkLength);
			json = nlohmann::json::parse(jsonText);
		}

		{ // le second de type BIN
			std::uint32_t chunkLength = 0;
			file.read(reinterpret_cast<char *>(&chunkLength), sizeof(chunkLength));
			std::uint32_t chunkType = 0;
			file.read(reinterpret_cast<char *>(&chunkType), sizeof(chunkType));

			if(chunkType != 0x004E4942) return; // "BIN"

			p->raw.resize(chunkLength);
			file.read(reinterpret_cast<char *>(p->raw.data()), chunkLength);
		}

		//std::cout << json.dump(4) << std::endl; pretty print json

		if(!json["meshes"].is_array() ||
			!json["accessors"].is_array() ||
			!json["bufferViews"].is_array())
			return;

		std::size_t vertexBufferIndex = -1;
		std::size_t normalBufferIndex = -1;
		std::size_t tangentBufferIndex = -1;
		std::size_t uv0BufferIndex = -1;
		std::size_t uv1BufferIndex = -1;
		std::size_t uv2BufferIndex = -1;
		std::size_t indicesBufferIndex = -1;
		{
			JSON &jsonAttrs = json["meshes"][0]["primitives"][0]["attributes"];

			if(jsonAttrs["POSITION"].is_number_integer()) vertexBufferIndex = jsonAttrs["POSITION"];
			if(jsonAttrs["NORMAL"].is_number_integer()) normalBufferIndex = jsonAttrs["NORMAL"];
			if(jsonAttrs["TANGENT"].is_number_integer()) tangentBufferIndex = jsonAttrs["TANGENT"];
			if(jsonAttrs["TEXCOORD_0"].is_number_integer()) uv0BufferIndex = jsonAttrs["TEXCOORD_0"];
			if(jsonAttrs["TEXCOORD_1"].is_number_integer()) uv1BufferIndex = jsonAttrs["TEXCOORD_1"];
			if(jsonAttrs["TEXCOORD_2"].is_number_integer()) uv2BufferIndex = jsonAttrs["TEXCOORD_2"];

			JSON &jsonIndices = json["meshes"][0]["primitives"][0]["indices"];
			if(jsonIndices.is_number_integer()) indicesBufferIndex = jsonIndices;
		}

		{
			JSON &views = json["bufferViews"];
			JSON &accessors = json["accessors"];

			affectBL(p->vertex, accessors, views, vertexBufferIndex);
			affectBL(p->normal, accessors, views, normalBufferIndex);
			affectBL(p->tangent, accessors, views, tangentBufferIndex);
			affectBL(p->uv0, accessors, views, uv0BufferIndex);
			affectBL(p->uv1, accessors, views, uv1BufferIndex);
			affectBL(p->uv2, accessors, views, uv2BufferIndex);
			affectBL(p->index, accessors, views, indicesBufferIndex);
		}

		if(vertexBufferIndex != -1) {
			JSON &accessor = json["accessors"][vertexBufferIndex];

			mesh->mins.x = accessor["min"][0];
			mesh->mins.y = accessor["min"][1];
			mesh->mins.z = accessor["min"][2];

			mesh->maxs.x = accessor["max"][0];
			mesh->maxs.y = accessor["max"][1];
			mesh->maxs.z = accessor["max"][2];

			mesh->radius = max(mesh->maxs.x, max(mesh->maxs.y, mesh->maxs.z));
			mesh->radius = max(mesh->radius, max(abs(mesh->mins.x), max(abs(mesh->mins.y), abs(mesh->mins.z))));

			mesh->squaredRadius = mesh->radius * mesh->radius;
		}

	}, [=]() {
		if(p->vertex.byteLength) {
			auto vertexVBO = createPtr<VBO>(3, StaticMesh::Attributes::POSITION);
			vertexVBO->setRawData(
				p->raw.data() + p->vertex.byteOffset,
				static_cast<int>(p->vertex.byteLength),
				0, sizeof(float) * 3
			);
			mesh->addVBO(vertexVBO);
		}

		if(p->normal.byteLength) {
			auto normalVBO = createPtr<VBO>(3, StaticMesh::Attributes::NORMAL);
			normalVBO->setRawData(
				p->raw.data() + p->normal.byteOffset,
				static_cast<int>(p->normal.byteLength),
				0, sizeof(float) * 3
			);
			mesh->addVBO(normalVBO);
		}

		if(p->tangent.byteLength) {
			auto tangentVBO = createPtr<VBO>(3, StaticMesh::Attributes::TANGENT);

			std::vector<Vec3> tangents;
			tangents.reserve(p->tangent.byteLength / 3);

			std::uint8_t *start = p->raw.data() + p->tangent.byteOffset;
			std::uint8_t *end = start + p->tangent.byteLength;
			for(std::uint8_t *i = start; i < end; i += (4 * sizeof(float))) {
				tangents.push_back(reinterpret_cast<Vec3 &>(*i));
			}

			tangentVBO->setData(tangents);
			mesh->addVBO(tangentVBO);
		}

		if(p->uv0.byteLength) {
			auto uvVBO = createPtr<VBO>(2, StaticMesh::Attributes::UV_0);
			uvVBO->setRawData(
				p->raw.data() + p->uv0.byteOffset,
				static_cast<int>(p->uv0.byteLength),
				0, sizeof(float) * 2
			);
			mesh->addVBO(uvVBO);
		}

		if(p->uv1.byteLength) {
			auto uvVBO = createPtr<VBO>(2, StaticMesh::Attributes::UV_1);
			uvVBO->setRawData(
				p->raw.data() + p->uv1.byteOffset,
				static_cast<int>(p->uv1.byteLength),
				0, sizeof(float) * 2
			);
			mesh->addVBO(uvVBO);
		}

		if(p->uv2.byteLength) {
			auto uvVBO = createPtr<VBO>(2, StaticMesh::Attributes::UV_2);
			uvVBO->setRawData(
				p->raw.data() + p->uv2.byteOffset,
				static_cast<int>(p->uv2.byteLength),
				0, sizeof(float) * 2
			);
			mesh->addVBO(uvVBO);
		}

		if(p->index.byteLength) {
			auto ibo = createPtr<IBO>();
			ibo->setDataType(p->index.componentType);

			ibo->setRawData(
				p->raw.data() + p->index.byteOffset,
				static_cast<int>(p->index.byteLength),
				0, sizeof(unsigned short)
			);
			mesh->setIBO(ibo);
		}

		mesh->prepare();
		
		cb.success();
	});
}
