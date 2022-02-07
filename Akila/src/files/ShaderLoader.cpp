#include "Akila/files/ShaderLoader.hpp"

#include "Akila/files/FileSystem.hpp"
#include <fstream>
#include "Akila/graphics/ShaderBuilder.hpp"

using namespace Akila;

Shader *ShaderLoader::create(std::string const &path) {
	std::ifstream file;
	file.open(FileSystem::path(path).c_str());
	if(!file.good()) std::cerr << "Shader loading error : can't read " << path << std::endl;

	std::stringstream stream;
	stream << file.rdbuf();
	file.close();

	return ShaderBuilder::build(stream.str());
}
