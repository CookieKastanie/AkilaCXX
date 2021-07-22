#include "Demo/LoadingState.hpp"

#include "Akila/graphics/pbr/Environment.hpp"

#include "Akila/graphics/gl/Error.hpp"

LoadingState::LoadingState(): Akila::State{} {
	Akila::Core::display->setTitle("Demo Akila (o.o)");

	Akila::Core::resourcesBucket->setTexture("brdfLUT", Akila::Environment::createBRDFLUT());
	GL_ERROR_STACK();


	Akila::Core::resourcesBucket->loadResourceFile("main.res", []() -> void {
		std::cout << "Fin chargement main.res" << std::endl;
	});

	GL_ERROR_STACK();
	defaultTriangle = Akila::Core::resourcesBucket->getMesh("defaultTriangle");
	GL_ERROR_STACK();

	

	Akila::Core::renderer->setSharedCamera(std::make_shared<Akila::PerspectiveCamera>());

	Akila::Core::display->getKeybord()->onKeyPress([](Akila::Keyboard::Key key) -> void {
		if(key == Akila::Keyboard::TAB) Akila::Core::display->setFullscreen(!Akila::Core::display->isFullscreen());
	});
}

void LoadingState::update() {
	auto &cam = Akila::Core::renderer->getSharedCamera();

	float x = std::cos(Akila::Time::now) * 3.f;
	float z = std::sin(Akila::Time::now) * 3.f;

	cam->setPosition({x, 0.5, z});
	cam->update();
}

void LoadingState::draw() {
	Akila::Core::renderer->useDefaultFrameBuffer();

	Akila::Core::renderer->disable(Akila::Renderer::DEPTH_TEST);
	Akila::Core::renderer->disable(Akila::Renderer::CULL_FACE);
	Akila::Core::renderer->render(Akila::Core::resourcesBucket->getMaterial("loadingScreen").get(), defaultTriangle.get());

	

	Akila::Core::renderer->enable(Akila::Renderer::DEPTH_TEST);
	
	Akila::Core::renderer->clearDepth();

	Akila::Core::renderer->render(Akila::Core::resourcesBucket->getMaterial("brdfLUT").get(), defaultTriangle.get());
	Akila::Core::renderer->enable(Akila::Renderer::CULL_FACE);

	Akila::Core::renderer->render(
		Akila::Core::resourcesBucket->getMaterial("sword").get(),
		Akila::Core::resourcesBucket->getMesh("sword").get()
	);

	if(Akila::Core::display->getKeybord()->isPressed(Akila::Keyboard::Key::A)) std::cout << "A" << std::endl;
	if(Akila::Core::display->getKeybord()->isPressed(Akila::Keyboard::Key::Q)) std::cout << "Q" << std::endl;
	if(Akila::Core::display->getKeybord()->isPressed(Akila::Keyboard::Key::UP)) std::cout << "UP" << std::endl;
	if(Akila::Core::display->getKeybord()->isPressed(Akila::Keyboard::Key::SPACE)) std::cout << "SPACE" << std::endl;
}
