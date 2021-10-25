#include "Akila/files/ResourceFile.hpp"
#include "Akila/files/FileSystem.hpp"
#include "Akila/files/Loader.hpp"

using namespace Akila;

enum class State: int {
	NONE,
	SHADER,
	TEXTURE,
	MESH,
	MATERIAL
};

void ResourceFile::unserializeFrom(const std::string path) {
	std::ifstream file;
	file.open(FileSystem::path(path).c_str(), std::ifstream::in);
	if(!file.good()) {
		std::cerr << "Resource list loading error : can't read " << path << std::endl;
		return;
	}

	State state = State::NONE;
	ShaderInfos *shaderInfos = nullptr;
	TextureInfos *textureInfos = nullptr;
	MeshInfos *meshInfos = nullptr;
	MaterialInfos *materialInfos = nullptr;

	std::string line;
	while(std::getline(file, line)) {
		std::vector<std::string> strs;
		Loader::splitString(strs, line, " ");
		for(auto &s : strs) Loader::trimString(s);



		if(state == State::NONE) {
			if(!strs.empty()) {

			}
		} else {

		}
	}
}
