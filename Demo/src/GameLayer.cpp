#include "Demo/GameLayer.hpp"
#include "Demo/MouseCamera.hpp"
#include <nlohmann/json.hpp>
#include <imgui/imgui.h>
#include <Akila/core/Metrics.hpp>
#include <Akila/graphics/MeshPrimitives.hpp>

using namespace Akila;
using nlohmann::json;

GameLayer::GameLayer() {
	//Core::resourcePool->meshs.set("sword", MeshPrimitives::invertedCube());
	//swordMaterial = Core::resourcePool->materials.get("redColor");
	//sword = Core::resourcePool->meshs.get("glbCube");

	sword = Core::resourcePool->meshs.get("sword");
	swordMaterial = Core::resourcePool->materials.get("sword");

	Core::renderer->setCamera(createPtr<MouseCamera>(Core::display->getMouse()));
	Core::renderer->setClearColor(0, 0, 0);

	Core::display->getKeybord()->onKeyPress([](Keyboard::Key key) {
		if(key == Keyboard::Key::SPACE) {
			json file = {
				{"shaders", {
					{
						{"name", "pbr"},
						{"src", "shaders/pbr.glsl"},
						{"uniforms-f", {
							{"exposure", 1},
							{"gamma", 2.2}
						}},
						{"uniforms-i", {
							{"tonemapping", 0},
							{"albedoSampler", 0},
							{"normalSampler", 1},
							{"metallicSampler", 2},
							{"roughnessSampler", 3},
							{"aoSampler", 4},
							{"irradianceMap", 5},
							{"prefilterMap", 6},
							{"brdfLUT", 7}
						}}
					}
				}}
			};

			Core::resourcePool->load(file, []() {
				LOG("pbr shader reloaded");
			});
		}
	});
}

void GameLayer::update() {

}

void GameLayer::draw() {
	FUNC_TIME_METRIC();
	
	Core::renderer->useDefaultFrameBuffer();

	//Core::renderer->depthFunc(Renderer::DepthFunc::LESS);
	//Core::renderer->disable(Renderer::Capability::CULL_FACE);
	Core::renderer->enable(Akila::Renderer::Capability::DEPTH_TEST);
	Core::renderer->clear();

	Core::renderer->render(swordMaterial, sword);
}

void GameLayer::drawImGui() {
	FUNC_TIME_METRIC();

	ImGui::Begin("Test");
	ImGui::End();

	ImGui::Begin("Test2");
	ImGui::End();
}

