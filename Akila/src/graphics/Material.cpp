#include "Akila/graphics/Material.hpp"
#include "Akila/graphics/ShaderBuilder.hpp"
#include "Akila/files/FileSystem.hpp"

using namespace Akila;

Material::Material(): name{"unnamed"} {}

void Material::loadFromFile(const std::string &fileName) {
	std::ifstream file;
	file.open(FileSystem::path(fileName).c_str(), std::ifstream::in);
	if(!file.good()) std::cerr << "Material loading error : can't read " << fileName << std::endl;
	
	std::string line;
	while(std::getline(file, line)) {
		std::vector<std::string> values;
		FileSystem::splitString(values, line, "=");
		if(values.size() > 1) {
			if(values[0].compare("name") == 0) name = values[1];
			else if(values[0].compare("shader") == 0) shader = ShaderBuilder::buildFromFile(values[1]);
		}
	}

	file.close();

	if(shader.get() == nullptr) {
		std::cerr << name << " material don't have shader program !" << std::endl;
		shader = std::make_shared<Shader>("", "");
	}
}

Shader *Material::getShader() {
	return shader.get();
}

std::string &Material::getName() {
	return name;
}
