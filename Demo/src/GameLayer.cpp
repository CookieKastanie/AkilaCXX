#include "Demo/GameLayer.hpp"
#include "Demo/MouseCamera.hpp"
#include <Akila/files/ResourceFileLoader.hpp>
#include <nlohmann/json.hpp>
#include <imgui/imgui.h>

using nlohmann::json;

GameLayer::GameLayer() {
	sword = Akila::Core::resourcePool->meshs.get("sword");
	swordMaterial = Akila::Core::resourcePool->materials.get("sword");

	Akila::Core::renderer->setSharedCamera(std::make_shared<MouseCamera>(Akila::Core::display->getMouse()));
	Akila::Core::renderer->setClearColor(0, 0, 0);

	Akila::Core::display->getKeybord()->onKeyPress([](Akila::Keyboard::Key key) {
		if(key == Akila::Keyboard::Key::SPACE) {
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

			Akila::ResourceFileLoader::fillResourcePool(Akila::Core::resourcePool.get(), file, []() {
				LOG("pbr shader reloaded");
			});
		}
	});
}

void GameLayer::update() {

}

void GameLayer::draw() {
	Akila::Core::renderer->useDefaultFrameBuffer();

	//Akila::Core::renderer->depthFunc(Akila::Renderer::DepthFunc::LESS);
	//Akila::Core::renderer->disable(Akila::Renderer::Capability::CULL_FACE);
	Akila::Core::renderer->enable(Akila::Renderer::Capability::DEPTH_TEST);
	Akila::Core::renderer->clear();

	Akila::Core::renderer->render(swordMaterial.raw(), sword.raw());
}

void GameLayer::drawImGui() {
	ImGui::Begin("Test");
	ImGui::End();

	ImGui::Begin("Test2");
	ImGui::End();
}

