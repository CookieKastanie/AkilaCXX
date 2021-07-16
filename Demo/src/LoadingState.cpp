#include "Demo/LoadingState.hpp"

LoadingState::LoadingState(): Akila::State{} {
	Akila::Core::display->setTitle("Demo Akila (o.o)");

	Akila::Core::resourcesBucket->loadResourceFile("main.res");
	defaultTriangle = Akila::Core::resourcesBucket->getMesh("defaultTriangle");

	Akila::Core::display->getKeybord()->onKeyPress([](Akila::Keyboard *keyboard) -> void {
		if(keyboard->isPressed(Akila::Keyboard::TAB)) Akila::Core::display->setFullscreen(!Akila::Core::display->isFullscreen());
	});
}

void LoadingState::update() {
	auto &cam = Akila::Core::renderer->getSharedCamera();

	float x = std::cos(Akila::Time::now);
	float y = std::cos(Akila::Time::now);

	cam->setPosition({x, y, 0.5});
}

void LoadingState::draw() {
	Akila::Core::renderer->useDefaultFrameBuffer();
	Akila::Core::renderer->render(Akila::Core::resourcesBucket->getMaterial("loadingScreen").get(), defaultTriangle.get());

	Akila::Core::renderer->render(Akila::Core::resourcesBucket->getMaterial("textureTest").get(), defaultTriangle.get());

	if(Akila::Core::display->getKeybord()->isPressed(Akila::Keyboard::Key::A)) std::cout << "A" << std::endl;
	if(Akila::Core::display->getKeybord()->isPressed(Akila::Keyboard::Key::Q)) std::cout << "Q" << std::endl;
	if(Akila::Core::display->getKeybord()->isPressed(Akila::Keyboard::Key::UP)) std::cout << "UP" << std::endl;
	if(Akila::Core::display->getKeybord()->isPressed(Akila::Keyboard::Key::SPACE)) std::cout << "SPACE" << std::endl;
}
