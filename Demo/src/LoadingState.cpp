#include "Demo/LoadingState.hpp"
#include "Akila/core/Core.hpp"
#include "Akila/core/Time.hpp"
#include <iostream>
#include <string>

#include <chrono>
#include <stdlib.h>

#include "Akila/graphics/ShaderBuilder.hpp"
#include <vector>
#include "glm/vec2.hpp"

LoadingState::LoadingState(): Akila::State{} {
	Akila::Core::display->setTitle("Demo Akila (o.o)");

	vertex = std::make_shared<Akila::VBO>(2, Akila::ShaderBuilder::Attributes::A_POSITION);
	vertex->setData(std::vector<glm::vec2>({
		{-1, -1}, {1, -1}, {1, 1},
		{-1, -1}, {1, 1}, {-1, 1}
	}));

	uv = std::make_shared<Akila::VBO>(2, Akila::ShaderBuilder::Attributes::A_UV);
	uv->setData(std::vector<glm::vec2>({
		{0, 0}, {1, 0}, {1, 1},
		{0, 0}, {1, 1}, {0, 1}
	}));

	vao = std::make_shared<Akila::VAO>();
	vao->bind();
	vao->registerVBO(vertex.get());
	vao->registerVBO(uv.get());
	vao->unbind();

	Akila::Core::display->getKeybord()->onKeyPress([](Akila::Keyboard *keyboard) -> void {
		if(keyboard->isPressed(Akila::Keyboard::TAB)) Akila::Core::display->setFullscreen(!Akila::Core::display->isFullscreen());
	});

	Akila::Core::resourcesBucket->loadResourceFile("main.res");
}

void LoadingState::update() {
	auto &cam = Akila::Core::renderer->getSharedCamera();

	float t = std::sin(Akila::Time::now);
	glm::vec3 pos{t * t, 0, 0};
	cam->setPosition(pos);
}

void LoadingState::draw() {
	Akila::Core::renderer->useDefaultFrameBuffer();
	Akila::Core::renderer->render(Akila::Core::resourcesBucket->getMaterial("loadingScreen").get(), vao.get());;

	//Akila::Core::resourcesBucket->getMaterial("textureTest")->getShader()->bind();
	//Akila::Core::resourcesBucket->getTexture("citron")->bind();
	//vao->draw();

	if(Akila::Core::display->getKeybord()->isPressed(Akila::Keyboard::Key::A)) std::cout << "A" << std::endl;
	if(Akila::Core::display->getKeybord()->isPressed(Akila::Keyboard::Key::Q)) std::cout << "Q" << std::endl;
	if(Akila::Core::display->getKeybord()->isPressed(Akila::Keyboard::Key::UP)) std::cout << "UP" << std::endl;
	if(Akila::Core::display->getKeybord()->isPressed(Akila::Keyboard::Key::SPACE)) std::cout << "SPACE" << std::endl;

	//else std::cout << std::endl;
}
