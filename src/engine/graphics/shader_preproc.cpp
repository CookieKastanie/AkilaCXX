#include "akila/engine/graphics/shader_preproc.hpp"
#include "akila/core/resource/file_system.hpp"
#include <iostream>
#include <regex>

using namespace akila;

bool ShaderPreproc::Define::operator<(ShaderPreproc::Define const &other) const {
	return name < other.name;
}

std::string ShaderPreproc::version = "#version 450\n";
std::set<ShaderPreproc::Define> ShaderPreproc::defines{};

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

void ShaderPreproc::parseLine(std::string &line, std::string *&currentSource, ShaderSources &sources) {
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

		// !!!!!!!!!!!!!!!
		if(line.compare("#akila_user_code")) currentSource->append(line + "\n");
	}
}

void ShaderPreproc::process(std::string const &source, ShaderSources &sources, std::string const &currentPath) {
	(void)currentPath;

	std::istringstream iss(source);

	std::string garbage;
	std::string *currentSource = &garbage;

	std::string shaderTemplatePath = "";

	std::string line;
	if(std::getline(iss, line)) {
		if(line.find("#akila_template") != std::string::npos) {
			shaderTemplatePath = line.substr(line.find(" ") + 1);
		}
	}

	do {
		parseLine(line, currentSource, sources);
	} while(std::getline(iss, line));

	if(!shaderTemplatePath.empty()) {
		std::string templateSource = readShaderSource(shaderTemplatePath);
		std::istringstream iss2(templateSource);

		ShaderSources templateSources;
		currentSource = &garbage;

		while(std::getline(iss2, line)) {
			parseLine(line, currentSource, templateSources);
			if(!line.compare("#akila_user_code")) {
				if(&templateSources.vertexShader == currentSource)
					currentSource->append(sources.vertexShader);
				else if(&templateSources.fragmentShader == currentSource)
					currentSource->append(sources.fragmentShader);
				else if(&templateSources.geometryShader == currentSource)
					currentSource->append(sources.geometryShader);
			}
		}

		sources.vertexShader = version + templateSources.vertexShader;
		sources.fragmentShader = version + templateSources.fragmentShader;
		if(!templateSources.geometryShader.empty()) sources.geometryShader = version + templateSources.geometryShader;

		sources.templateName = shaderTemplatePath;
	} else {
		sources.vertexShader = version + sources.vertexShader;
		sources.fragmentShader = version + sources.fragmentShader;
		if(!sources.geometryShader.empty()) sources.geometryShader = version + sources.geometryShader;
	}
}
