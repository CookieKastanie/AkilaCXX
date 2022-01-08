#include "Demo/PlayLayer.hpp"

#include "Akila/graphics/pbr/Environment.hpp"
#include "Akila/graphics/gl/Texture.hpp"
#include "imgui/imgui.h"

#include "Akila/graphics/gl/Error.hpp"

PlayLayer::PlayLayer(): Akila::Layer{}, exposure{1.} {
	//Akila::Core::renderer->setSharedCamera(std::make_shared<Akila::PerspectiveCamera>());
	Akila::Core::renderer->setSharedCamera(std::make_shared<MouseCamera>(Akila::Core::display->getMouse()));

	Net::fillGrid(net);
}

void PlayLayer::update() {
	net.update();
}

void PlayLayer::draw() {
	auto& cam = Akila::Core::renderer->getSharedCamera();
	cam->update();

	const auto k = Akila::Core::display->getKeybord();
	auto shader = Akila::Core::resourcePool->getShader("pbr");
	shader->bind();
	if(k->isPressed(Akila::Keyboard::Key::A)) {
		shader->send("tonemapping", 0);
	}
	if(k->isPressed(Akila::Keyboard::Key::Z)) {
		shader->send("tonemapping", 1);
	}
	if(k->isPressed(Akila::Keyboard::Key::E)) {
		shader->send("tonemapping", 2);
	}
	if(k->isPressed(Akila::Keyboard::Key::R)) {
		shader->send("tonemapping", 3);
	}

	//std::cout << shader->getUniformId("tonemapping") << std::endl;

	if(k->isPressed(Akila::Keyboard::Key::W)) {
		exposure += Akila::Time::delta;
	}
	if(k->isPressed(Akila::Keyboard::Key::X)) {
		exposure -= Akila::Time::delta;
	}
	{
		if(exposure < 0) exposure = 0;
		shader->send("exposure", exposure);
	}
	

	///////////////////////////////////////////////////////////////////////////////////////


	Akila::Core::renderer->useDefaultFrameBuffer();

	Akila::Core::renderer->enable(Akila::Renderer::Capability::DEPTH_TEST);
	Akila::Core::renderer->clearDepth();

	Akila::Core::renderer->depthFunc(Akila::Renderer::DepthFunc::LEQUAL);
	Akila::Core::renderer->render(
		Akila::Core::resourcePool->getMaterial("skybox").get(),
		Akila::Core::resourcePool->getMesh("invertedCube").get()
	);
	Akila::Core::renderer->depthFunc(Akila::Renderer::DepthFunc::LESS);

	Akila::Core::renderer->enable(Akila::Renderer::Capability::CULL_FACE);

	Akila::Core::renderer->render(
		Akila::Core::resourcePool->getMaterial("sword").get(),
		Akila::Core::resourcePool->getMesh("sword").get()
	);

	Akila::Core::renderer->render(
		Akila::Core::resourcePool->getMaterial("red").get(),
		Akila::Core::resourcePool->getMesh("invertedCube").get()
	);

	//*
	Akila::Core::renderer->render(
		Akila::Core::resourcePool->getMaterial("terrain").get(),
		Akila::Core::resourcePool->getMesh("terrain").get()
	);
	//*/

	net.draw();
}

void PlayLayer::drawImGui() {
	ImGui::Begin("WoulaTest");
	ImGui::End();
}
