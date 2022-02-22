#include "Demo/GabLayer.hpp"
#include "Demo/MouseCamera.hpp"
#include <imgui/imgui.h>

using namespace Akila;

GabLayer::GabLayer(): albedo{0, 0.16, 0.9}, metallic{0}, roughness{0.75}, ambiantColor{0.1, 0.1, 0.1}, exposure{1.2} {
	Core::resourcePool->load("gabRes.json");
	flatPBRShader = Core::resourcePool->shaders.get("flatPBR");
	simpleShader = Core::resourcePool->shaders.get("simple");
	cube = Core::resourcePool->meshs.get("cube");
	model = Core::resourcePool->meshs.get("model");

	auto cam = createPtr<MouseCamera>(Core::display->getMouse());
	cam->setCenter({0, 0.5, 0});
	Core::renderer->setCamera(cam);
	Core::renderer->setClearColor(0, 0, 0);

	lightPositions = {{4.2, 10, 5.6}, {-5.5, 2.1, 0}, {1.7, -2.7, -2.7}};
	lightColors = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
	lightPowers = {530, 270, 200};
}

void GabLayer::draw() {
	Core::renderer->useDefaultFrameBuffer();
	Core::renderer->clear();

	Transform modelTransform;

	flatPBRShader->bind();
	flatPBRShader->send("model", modelTransform.toMatrix());

	flatPBRShader->send("albedo", albedo);

	flatPBRShader->send("metallic", metallic);
	flatPBRShader->send("roughness", roughness);
	flatPBRShader->send("ambiantColor", ambiantColor);
	flatPBRShader->send("exposure", exposure);

	flatPBRShader->send("lightPositions[0]", lightPositions);
	flatPBRShader->send("lightColors[0]", lightColors);
	flatPBRShader->send("lightPowers[0]", lightPowers);

	model->draw();


	simpleShader->bind();
	for(std::size_t i = 0; i < lightPositions.size(); ++i) {
		modelTransform.setPosition(lightPositions[i]);
		modelTransform.setScale(0.1);

		simpleShader->send("color", lightColors[i]);
		simpleShader->send("model", modelTransform.toMatrix());

		cube->draw();
	}
}

void GabLayer::drawImGui() {
	ImGui::Begin("Material");
	ImGui::ColorEdit3("Albedo", &albedo[0]);
	ImGui::SliderFloat("Metallic", &metallic, 0, 1);
	ImGui::SliderFloat("roughness", &roughness, 0, 1);
	ImGui::End();

	ImGui::Begin("Lights");
	for(std::size_t i = 0; i < lightPositions.size(); ++i) {
		ImGui::DragFloat3(("Light position " + std::to_string(i)).c_str(), &lightPositions[i][0], 0.1, -20, 20);
		ImGui::ColorEdit3(("Light color " + std::to_string(i)).c_str(), &lightColors[i][0]);
		ImGui::SliderFloat(("Light power " + std::to_string(i)).c_str(), &lightPowers[i], 0, 1000);
		ImGui::Separator();
	}
	ImGui::End();

	ImGui::Begin("Others");
	ImGui::ColorEdit3("Ambiant color", &ambiantColor[0]);
	ImGui::SliderFloat("Exposure", &exposure, 0, 5);
	ImGui::End();
}
