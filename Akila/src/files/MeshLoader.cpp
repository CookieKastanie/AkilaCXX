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
