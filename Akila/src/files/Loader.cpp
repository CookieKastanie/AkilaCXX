#include "Akila/core/Core.hpp"
#include "Akila/files/Loader.hpp"
#include "Akila/files/FileSystem.hpp"
#include "stbimage/stb_image.h"
#include "Akila/graphics/ShaderBuilder.hpp"

using namespace Akila;


class LoaderTask: public Task {
protected:
	std::function<void()> callback;

public:
	LoaderTask(): callback{[]() {}} {

	}

	void setCallBack(const std::function<void()> &cb) {
		callback = cb;
	}
};


class TextureTask: public LoaderTask {
private:
	TextureBuffer::Format fileFormat;
	TextureBuffer::Type dataType;
	int textureNrChannels;

	int fwidth = 1;
	int fheight = 1;

	void *data;

	Texture *texture;
	const std::string path;

	bool generateMips;

public:
	TextureTask(Texture *texture, const std::string &path, const bool generateMips):
		fileFormat{TextureBuffer::RGBA},
		dataType{TextureBuffer::UNSIGNED_BYTE},
		textureNrChannels{3},
		fwidth{1},
		fheight{1},
		data{nullptr},
		texture{texture},
		path{path},
		generateMips{generateMips} {
	}

	void onBackground() override {
		stbi_set_flip_vertically_on_load(true);
		if(texture->getInternalFormat() == TextureBuffer::RGB16F || texture->getInternalFormat() == TextureBuffer::RGBA16F) {// <- pas fou
			data = stbi_loadf(FileSystem::path(path).c_str(), &fwidth, &fheight, &textureNrChannels, 0);
			dataType = TextureBuffer::FLOAT;
		} else {
			data = stbi_load(FileSystem::path(path).c_str(), &fwidth, &fheight, &textureNrChannels, 0);
		}

		switch(textureNrChannels) {
			case 1: fileFormat = TextureBuffer::RED; break;
			case 3: fileFormat = TextureBuffer::RGB; break;
			case 4: fileFormat = TextureBuffer::RGBA; break;
		}
	}

	void onMain() override {
		if(data != nullptr) {
			texture->setSize(fwidth, fheight);
			texture->setData(data, fileFormat, dataType);
			if(generateMips) texture->generateMipmap();

			stbi_image_free(data);
		} else {
			std::cerr << "Texture loading error : can't read " << path << std::endl;
		}

		callback();
	}
};

void Loader::asyncTexture(Texture *texture, const std::string &path, const bool generateMips, const std::function<void()> &cb, TaskManager *tm) {
	if(tm == nullptr) tm = Core::taskManager.get();
	TextureTask *t = new TextureTask(texture, path, generateMips);
	t->setCallBack(cb);
	tm->submit(t);
}


std::shared_ptr<Shader> Loader::shader(const std::string &path) {
	std::ifstream file;
	file.open(FileSystem::path(path).c_str());
	if(!file.good()) std::cerr << "Shader loading error : can't read " << path << std::endl;

	std::stringstream stream;
	stream << file.rdbuf();
	file.close();

	return ShaderBuilder::build(stream.str());
}




class MeshTask: public LoaderTask {
private:
	Mesh *mesh;
	const std::string path;

	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> normal;
	std::vector<glm::vec3> tangent;
	std::vector<glm::vec2> uv;

public:
	MeshTask(Mesh *mesh, const std::string &path): mesh{mesh}, path{path} {}

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

		return tangent;
		//return glm::normalize(tangent - glm::dot(tangent, n) * n);
	}

	void onBackground() override {
		std::ifstream file(FileSystem::path(path));
		if(!file.good()) std::cerr << "Mesh loading error : can't read " << path << std::endl;

		std::vector<glm::vec3> tmpVertex;
		std::vector<glm::vec3> tmpNormal; bool haveNormal = true;
		std::vector<glm::vec2> tmpUV; bool haveUV = true;
		std::vector<std::vector<int>> tmpFaces;

		std::string line;
		while(std::getline(file, line)) {
			if(line.empty()) continue;

			std::vector<std::string> list;
			Loader::splitString(list, line, " ");

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
					Loader::splitString(subStrs, list[i + 1], "/");
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
	}

	void onMain() override {
		auto vertexVBO = std::make_shared<VBO>(3, ShaderBuilder::Attributes::A_POSITION);
		vertexVBO->setData(vertex);
		mesh->addVBO(vertexVBO);

		if(uv.size() > 0) {
			auto uvVBO = std::make_shared<VBO>(2, ShaderBuilder::Attributes::A_UV);
			uvVBO->setData(uv);
			mesh->addVBO(uvVBO);
		}

		if(normal.size() > 0) {
			auto normalVBO = std::make_shared<VBO>(3, ShaderBuilder::Attributes::A_NORMAL);
			normalVBO->setData(normal);
			mesh->addVBO(normalVBO);
		}

		if(tangent.size() > 0) {
			auto tangentVBO = std::make_shared<VBO>(3, ShaderBuilder::Attributes::A_TANGENT);
			tangentVBO->setData(tangent);
			mesh->addVBO(tangentVBO);
		}

		mesh->prepare();

		callback();
	}
};

void Loader::asyncMesh(Mesh *mesh, const std::string &path, const std::function<void()> &cb, TaskManager *tm) {
	if(tm == nullptr) tm = Core::taskManager.get();
	MeshTask *t = new MeshTask(mesh, path);
	t->setCallBack(cb);
	tm->submit(t);
}















void Loader::splitString(std::vector<std::string> &list, std::string &str, const std::string &delimiter) {
	list.clear();

	std::size_t pos = 0;
	while((pos = str.find(delimiter)) != std::string::npos) {
		list.push_back(str.substr(0, pos));
		str.erase(0, pos + delimiter.length());
	}

	list.push_back(str);
}

void Loader::trimString(std::string &str) {
	std::size_t i = 0;

	while((i < str.size()) && std::isspace(str[i])) ++i;
	str.erase(0, i);

	if(str.size() == 0) return;

	i = str.size() - 1;

	while((i >= 0) && std::isspace(str[i])) --i;
	str.erase(i + 1, str.size());
}
