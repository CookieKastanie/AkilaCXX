#include "akila/renderer/shader_preproc.hpp"
#include "akila/resource/file_system.hpp"
#include <iostream>
#include <regex>

using namespace akila;

bool ShaderPreProc::Define::operator<(ShaderPreProc::Define const &other) const {
	return name < other.name;
}

std::string ShaderPreProc::version = "#version 450\n";
std::set<ShaderPreProc::Define> ShaderPreProc::defines{};

std::string readShaderSource(std::string const path) {
	std::ifstream file;
	file.open(FileSystem::path(path).c_str());
	if(!file.good()) {
		std::cerr << "Shader loading error : can't read " << FileSystem::path(path) << std::endl;
		return "";
	}

	std::stringstream stream;
	stream << file.rdbuf();
	return stream.str();
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
		
		// permet d'inclure d'autres fichiers (n'est pas recursif)
		else if(line.find("#akila_file") != std::string::npos)
			currentSource->append(
				// split apres le premier espace
				readShaderSource(line.substr(
					line.find(" ") + 1)
				)
			);

		else {
			for(Define const &define : defines) {
				line = std::regex_replace(line, std::regex(define.name), define.value);
			}

			currentSource->append(line + "\n");
		}
	}

	sources.vertexShader = version + sources.vertexShader;
	sources.fragmentShader = version + sources.fragmentShader;
	if(!sources.geometryShader.empty()) sources.geometryShader = version + sources.geometryShader;
}
