#include "Demo/PlayState.hpp"

#include "Akila/graphics/pbr/Environment.hpp"
#include "Akila/graphics/gl/Texture.hpp"


#include "Akila/graphics/gl/Error.hpp"

PlayState::PlayState(): Akila::State{} {
	//Akila::Core::renderer->setSharedCamera(std::make_shared<Akila::PerspectiveCamera>());
	Akila::Core::renderer->setSharedCamera(std::make_shared<MouseCamera>(Akila::Core::display->getMouse()));
}

void PlayState::update() {
	auto &cam = Akila::Core::renderer->getSharedCamera();
	cam->update();
}

void PlayState::draw() {
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
}
