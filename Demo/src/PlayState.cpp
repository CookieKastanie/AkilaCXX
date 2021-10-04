#include "Demo/PlayState.hpp"

#include "Akila/graphics/pbr/Environment.hpp"
#include "Akila/graphics/gl/Texture.hpp"


#include "Akila/graphics/gl/Error.hpp"

PlayState::PlayState(): Akila::State{}, exposure{1.} {
	//Akila::Core::renderer->setSharedCamera(std::make_shared<Akila::PerspectiveCamera>());
	Akila::Core::renderer->setSharedCamera(std::make_shared<MouseCamera>(Akila::Core::display->getMouse()));

	Net::fillGrid(net);
}

void PlayState::update() {
	net.update();
}

void PlayState::draw() {
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

	Akila::Core::renderer->enable(Akila::Renderer::DEPTH_TEST);
	Akila::Core::renderer->clearDepth();

	glDepthFunc(GL_LEQUAL);
	Akila::Core::renderer->render(
		Akila::Core::resourcePool->getMaterial("skybox").get(),
		Akila::Core::resourcePool->getMesh("invertedCube").get()
	);
	glDepthFunc(GL_LESS);

	Akila::Core::renderer->enable(Akila::Renderer::CULL_FACE);

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
