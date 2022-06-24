#include "akila/renderer/shader_preproc.hpp"
#include "akila/resource/file_system.hpp"
#include <iostream>

using namespace akila;

std::string ShaderPreProc::version = "#version 450\n";

void ShaderPreProc::readShaderSource(std::string const path) {
	std::ifstream file;
	file.open(FileSystem::path(path).c_str());
	if(!file.good()) {
		std::cerr << "Shader loading error : can't read " << path << std::endl;
		return;
	}

	std::stringstream stream;
	stream << file.rdbuf();
	file.close();
}

void ShaderPreProc::process(std::string const &source, ShaderSources &sources, std::string const &currentPath) {
	std::istringstream iss(source);

	std::string garbage;
	std::string *currentSource = &garbage;

	std::string line;
	while(std::getline(iss, line)) {
		if(!line.compare("#akila_vertex")) currentSource = &sources.vertexShader;
		else if(!line.compare("#akila_fragment")) currentSource = &sources.fragmentShader;
		else if(!line.compare("#akila_geometry")) currentSource = &sources.geometryShader;
		else currentSource->append(line + "\n");
	}

	sources.vertexShader = version + sources.vertexShader;
	sources.fragmentShader = version + sources.fragmentShader;
	if(!sources.geometryShader.empty()) sources.geometryShader = version + sources.geometryShader;
}
