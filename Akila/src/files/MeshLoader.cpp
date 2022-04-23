#include "Akila/files/MeshLoader.hpp"

#include "Akila/core/Core.hpp"
#include "Akila/graphics/ShaderBuilder.hpp"

using namespace Akila;

void splitString(std::vector<std::string> &list, std::string &str, const std::string &delimiter) {
	list.clear();

	std::size_t pos = 0;
	while((pos = str.find(delimiter)) != std::string::npos) {
		list.push_back(str.substr(0, pos));
		str.erase(0, pos + delimiter.length());
	}

	list.push_back(str);
}

glm::vec3 calcTangent(glm::vec3 &n, glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3, glm::vec2 &uv1, glm::vec2 &uv2, glm::vec2 &uv3) {
	glm::vec3 edge1 = p2 - p1;
	glm::vec3 edge2 = p3 - p1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	float f = 1. / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	glm::vec3 tangent;

	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

	//return tangent;
	return glm::normalize(tangent - glm::dot(tangent, n) * n);
}

void MeshLoader::obj(Mesh *mesh, std::string const &path, std::function<void()> const &callback) {
	struct _ {
		Mesh *mesh;
		std::string const path;
		std::function<void()> const callback;
		std::vector<glm::vec3> vertex;
		std::vector<glm::vec3> normal;
		std::vector<glm::vec3> tangent;
		std::vector<glm::vec2> uv;
	};
	auto coro = Core::coroutines->create<>(_{mesh, path, callback});

	coro->pushInThread(Trigger::AT_FRAME_START, [](_ &state) {
		std::ifstream file(FileSystem::path(state.path));
		if(!file.good()) std::cerr << "Mesh loading error : can't read " << state.path << std::endl;

		auto &vertex = state.vertex;
		auto &normal = state.normal;
		auto &tangent = state.tangent;
		auto &uv = state.uv;

		std::vector<glm::vec3> tmpVertex;
		std::vector<glm::vec3> tmpNormal; bool haveNormal = true;
		std::vector<glm::vec2> tmpUV; bool haveUV = true;
		std::vector<std::vector<int>> tmpFaces;

		std::string line;
		while(std::getline(file, line)) {
			if(line.empty()) continue;

			std::vector<std::string> list;
			splitString(list, line, " ");

			if(list[0].compare("v") == 0) {
				if(list.size() < 4) continue;
				tmpVertex.push_back(glm::vec3(std::stof(list[1]), std::stof(list[2]), std::stof(list[3])));
			} else if(list[0].compare("vn") == 0) {
				if(list.size() < 4) continue;
				tmpNormal.push_back(glm::vec3(std::stof(list[1]), std::stof(list[2]), std::stof(list[3])));
			} else if(list[0].compare("vt") == 0) {
				if(list.size() < 3) continue;
				tmpUV.push_back(glm::vec2(std::stof(list[1]), std::stof(list[2])));
			} else if(list[0].compare("f") == 0) {
				if(list.size() < 4) continue;

				for(int i = 0; i < 3; ++i) {
					std::vector<std::string> subStrs;
					splitString(subStrs, list[i + 1], "/");
					if(subStrs.size() < 1) continue;

					if(subStrs.size() < 2 || subStrs[1].empty()) haveUV = false;
					if(subStrs.size() < 3 || subStrs[2].empty()) haveNormal = false;

					std::vector<int> ids{
						std::stoi(subStrs[0]) - 1,
						haveUV ? std::stoi(subStrs[1]) - 1 : -1,
						haveNormal ? std::stoi(subStrs[2]) - 1 : -1
					};

					tmpFaces.push_back(ids);
				}
			}
		}

		file.close();


		for(std::vector<int> &face : tmpFaces) {
			vertex.push_back(tmpVertex[face[0]]);
			if(haveUV) uv.push_back(tmpUV[face[1]]);
			if(haveNormal) normal.push_back(tmpNormal[face[2]]);
		}

		if(haveNormal && haveUV) {
			for(int i = 0; i < vertex.size(); i += 3) {
				glm::vec3 &p1 = vertex[i];
				glm::vec3 &p2 = vertex[i + 1];
				glm::vec3 &p3 = vertex[i + 2];

				glm::vec3 &n1 = normal[i];
				glm::vec3 &n2 = normal[i + 1];
				glm::vec3 &n3 = normal[i + 2];

				glm::vec2 &uv1 = uv[i];
				glm::vec2 &uv2 = uv[i + 1];
				glm::vec2 &uv3 = uv[i + 2];

				tangent.push_back(calcTangent(n1, p1, p2, p3, uv1, uv2, uv3));
				tangent.push_back(calcTangent(n2, p2, p3, p1, uv2, uv3, uv1));
				tangent.push_back(calcTangent(n3, p3, p1, p2, uv3, uv1, uv2));
			}
		} else if(haveNormal) {
			for(glm::vec3 &n : normal) {
				tangent.push_back({n.y, -n.x, n.z});
			}
		}

		return 1;
	});

	coro->push(Trigger::AT_FRAME_START, [](_ &state) {
		Mesh *mesh = state.mesh;
		auto &vertex = state.vertex;
		auto &normal = state.normal;
		auto &tangent = state.tangent;
		auto &uv = state.uv;

		auto vertexVBO = createPtr<VBO>(3, ShaderBuilder::Attributes::A_POSITION);
		vertexVBO->setData(vertex);
		mesh->addVBO(vertexVBO);

		if(uv.size() > 0) {
			auto uvVBO = createPtr<VBO>(2, ShaderBuilder::Attributes::A_UV);
			uvVBO->setData(uv);
			mesh->addVBO(uvVBO);
		}

		if(normal.size() > 0) {
			auto normalVBO = createPtr<VBO>(3, ShaderBuilder::Attributes::A_NORMAL);
			normalVBO->setData(normal);
			mesh->addVBO(normalVBO);
		}

		if(tangent.size() > 0) {
			auto tangentVBO = createPtr<VBO>(3, ShaderBuilder::Attributes::A_TANGENT);
			tangentVBO->setData(tangent);
			mesh->addVBO(tangentVBO);
		}

		mesh->prepare();

		state.callback();
		
		return 1;
	});

	Core::coroutines->start(coro);
}


#include "nlohmann/json.hpp"

struct BufferLocator {
	std::size_t byteLength = 0;
	std::size_t byteOffset = 0;
};
void affectBL(BufferLocator &bl, nlohmann::json &views, std::size_t index) {
	if(index != -1) {
		bl.byteLength = views[index]["byteLength"];
		bl.byteOffset = views[index]["byteOffset"];
	}
};

void MeshLoader::glb(Mesh *mesh, std::string const &path, std::function<void()> const &callback) {
	struct _ {
		Mesh *mesh;
		std::string const path;
		std::function<void()> const callback;

		std::vector<std::uint8_t> raw;

		BufferLocator vertex;
		BufferLocator normal;
		BufferLocator tangent;
		BufferLocator uv;
		BufferLocator index;
	};
	auto coro = Core::coroutines->create<>(_{mesh, path, callback});

	coro->pushInThread(Trigger::AT_FRAME_START, [](_ &state) {
		std::ifstream file(FileSystem::path(state.path), std::ios::binary);
		if(!file.good()) std::cerr << "Mesh loading error : can't read " << state.path << std::endl;

		{
			std::uint32_t val;

			file.read(reinterpret_cast<char *>(&val), sizeof(val));
			if(val != 0x46546C67) return 1; // "glTF"

			file.read(reinterpret_cast<char *>(&val), sizeof(val));
			// verifier la version ?
			//std::cout << "version : " << val << std::endl;

			file.read(reinterpret_cast<char *>(&val), sizeof(val));
			// verifier la longueur a la fin ?
			//std::cout << "length : " << val << std::endl;
		}
		
		nlohmann::json json;
		{ // le premier block doit etre de type JSON
			std::uint32_t chunkLength;
			file.read(reinterpret_cast<char *>(&chunkLength), sizeof(chunkLength));
			std::uint32_t chunkType;
			file.read(reinterpret_cast<char *>(&chunkType), sizeof(chunkType));

			if(chunkType != 0x4E4F534A) return 1; // "JSON"

			std::string jsonText;
			jsonText.resize(chunkLength);
			file.read(jsonText.data(), chunkLength);
			json = nlohmann::json::parse(jsonText);
		}

		{ // le second de type BIN
			std::uint32_t chunkLength;
			file.read(reinterpret_cast<char *>(&chunkLength), sizeof(chunkLength));
			std::uint32_t chunkType;
			file.read(reinterpret_cast<char *>(&chunkType), sizeof(chunkType));

			if(chunkType != 0x004E4942) return 1; // "BIN"

			state.raw.resize(chunkLength);
			file.read(reinterpret_cast<char *>(state.raw.data()), chunkLength);
		}

		//std::cout << json.dump(4) << std::endl;

		if(!json["meshes"].is_array() ||
		   !json["accessors"].is_array() ||
		   !json["bufferViews"].is_array())
			return 1;
		
		std::size_t vertexBufferIndex = -1;
		std::size_t normalBufferIndex = -1;
		std::size_t tangentBufferIndex = -1;
		std::size_t uvBufferIndex = -1;
		std::size_t indicesBufferIndex = -1;
		{
			nlohmann::json &jsonAttrs = json["meshes"][0]["primitives"][0]["attributes"];

			if(jsonAttrs["POSITION"].is_number_integer()) vertexBufferIndex = jsonAttrs["POSITION"];
			if(jsonAttrs["NORMAL"].is_number_integer()) normalBufferIndex = jsonAttrs["NORMAL"];
			if(jsonAttrs["TANGENT"].is_number_integer()) tangentBufferIndex = jsonAttrs["TANGENT"];
			if(jsonAttrs["TEXCOORD_0"].is_number_integer()) uvBufferIndex = jsonAttrs["TEXCOORD_0"];

			nlohmann::json &jsonIndices = json["meshes"][0]["primitives"][0]["indices"];
			if(jsonIndices.is_number_integer()) indicesBufferIndex = jsonIndices;
		}
		
		{
			nlohmann::json &views = json["bufferViews"];

			affectBL(state.vertex, views, vertexBufferIndex);
			affectBL(state.normal, views, normalBufferIndex);
			affectBL(state.tangent, views, tangentBufferIndex);
			affectBL(state.uv, views, uvBufferIndex);
			affectBL(state.index, views, indicesBufferIndex);
		}

		return 1;
	});

	coro->push(Trigger::AT_FRAME_START, [](_ &state) {

		if(state.vertex.byteLength) {
			auto vertexVBO = createPtr<VBO>(3, ShaderBuilder::Attributes::A_POSITION);
			vertexVBO->setRawData(
				state.raw.data() + state.vertex.byteOffset,
				state.vertex.byteLength,
				0, sizeof(float) * 3
			);
			state.mesh->addVBO(vertexVBO);
		}

		if(state.normal.byteLength) {
			auto normalVBO = createPtr<VBO>(3, ShaderBuilder::Attributes::A_NORMAL);
			normalVBO->setRawData(
				state.raw.data() + state.normal.byteOffset,
				state.normal.byteLength,
				0, sizeof(float) * 3
			);
			state.mesh->addVBO(normalVBO);
		}

		if(state.tangent.byteLength) {
			auto tangentVBO = createPtr<VBO>(3, ShaderBuilder::Attributes::A_TANGENT);

			std::vector<glm::vec3> tangents;
			tangents.reserve(state.tangent.byteLength / 3);

			std::uint8_t *start = state.raw.data() + state.tangent.byteOffset;
			std::uint8_t *end = start + state.tangent.byteLength;
			for(std::uint8_t *i = start; i < end; i += (4 * sizeof(float))) {
				tangents.push_back(reinterpret_cast<glm::vec3&>(*i));
			}

			tangentVBO->setData(tangents);
			state.mesh->addVBO(tangentVBO);
		}

		if(state.uv.byteLength) {
			auto uvVBO = createPtr<VBO>(2, ShaderBuilder::Attributes::A_UV);
			uvVBO->setRawData(
				state.raw.data() + state.uv.byteOffset,
				state.uv.byteLength,
				0, sizeof(float) * 2
			);
			state.mesh->addVBO(uvVBO);
		}

		if(state.index.byteLength) {
			auto ibo = createPtr<IBO>();
			ibo->setRawData(
				state.raw.data() + state.index.byteOffset,
				state.index.byteLength,
				0, sizeof(unsigned short)
			);
			state.mesh->setIBO(ibo);
		}

		state.mesh->prepare();

		state.callback();

		return 1;
	});

	Core::coroutines->start(coro);
}
