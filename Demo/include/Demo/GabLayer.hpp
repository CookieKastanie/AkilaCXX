#pragma once

#include <Akila/core/Core.hpp>
#include <Akila/entities/Transform.hpp>
#include <vector>

using namespace Akila;

class GabLayer: public Layer {
private:
	Ref<Shader> flatPBRShader;
	Ref<Shader> simpleShader;

	Ref<Mesh> cube;
	Ref<Mesh> model;

	///

	glm::vec3 albedo;
	float metallic;
	float roughness;

	glm::vec3 ambiantColor;
	
	float exposure;

	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;
	std::vector<float> lightPowers;

public:
	GabLayer();

	void draw() override;
	void drawImGui() override;
};
