#include "Akila/graphics/ShaderBuilder.hpp"

#include "Akila/files/FileSystem.hpp"
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
struct akila_camera {
	mat4 projection;
	mat4 view;
	mat4 pv;
	vec3 position;
	float ratio;
};
layout(std140) uniform akila_camera_ubo {
	 akila_camera u_camera;
};

layout(std140) uniform akila_time_ubo {
	 float u_time;
};

const int AKILA_POINT_LIGHT_COUNT = 4;
struct akila_lights {
	vec3 directionalLight;
	vec3 directionalColor;

	vec3 pointsPositions[AKILA_POINT_LIGHT_COUNT];
	vec3 pointsColors[AKILA_POINT_LIGHT_COUNT];
};
layout(std140) uniform akila_lights_ubo {
	 akila_lights u_lights;
};
)---";

const std::string ShaderBuilder::functions = R"---(
const float PI = 3.1415926535897932;
)---";

const std::string ShaderBuilder::vertDelimiter = "#AKILA_VERTEX";
const std::string ShaderBuilder::fragDelimiter = "#AKILA_FRAGMENT";
const std::string ShaderBuilder::geoDelimiter = "#AKILA_GEOMETRY";

Shader *ShaderBuilder::build(const std::string &source) {
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

	return new Shader(vert, frag, geo);
}
