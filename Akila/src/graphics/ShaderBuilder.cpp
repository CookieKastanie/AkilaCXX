#include "Akila/graphics/ShaderBuilder.hpp"

#include "Akila/files/FileSystem.hpp"
#include <sstream>
#include <fstream>
#include <iostream>

using namespace Akila;

const std::string ShaderBuilder::version = R"---(#version 330 core
)---";

const std::string ShaderBuilder::attributes = R"---(
layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_uv;
layout(location = 2) in vec4 a_normal;
layout(location = 3) in vec4 a_tangent;
layout(location = 4) in vec4 a_color;
)---";

const std::string ShaderBuilder::uniforms = R"---(
layout(std140) uniform camera {
	mat4 projection;
	mat4 view;
	mat4 PV;
	vec3 position;
};
)---";

const std::string ShaderBuilder::functions = R"---(
)---";

const std::string ShaderBuilder::vertDelimiter = "#AKILA_VERTEX";
const std::string ShaderBuilder::fragDelimiter = "#AKILA_FRAGMENT";
const std::string ShaderBuilder::geoDelimiter = "#AKILA_GEOMETRY";

std::shared_ptr<Shader> ShaderBuilder::build(const std::string &source) {
	std::istringstream iss(source);

	std::string vert;
	std::string frag;
	std::string geo;

	std::string garbage;
	std::string *currentSource = &garbage;

	std::string line;
	while(std::getline(iss, line)) {
		if(line.compare(vertDelimiter) == 0) currentSource = &vert;
		else if(line.compare(fragDelimiter) == 0) currentSource = &frag;
		else if(line.compare(geoDelimiter) == 0) currentSource = &geo;
		else currentSource->append(line + "\n");
	}

	std::string vuf = version + uniforms + functions;

	vert = vuf + attributes + vert;
	frag = vuf + frag;
	if(!geo.empty()) geo = vuf + geo;

	std::cout << vert << std::endl;

	return std::make_shared<Shader>(vert, frag, geo);
}

std::shared_ptr<Shader> ShaderBuilder::buildFromFile(const std::string &path) {
	std::ifstream file;
	file.open(FileSystem::path(path).c_str());
	std::stringstream stream;
	stream << file.rdbuf();
	file.close();

	return build(stream.str());
}
