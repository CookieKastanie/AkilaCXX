#include "Demo/LoadingLayer.hpp"

#include "Demo/PlayLayer.hpp"

#include "Akila/graphics/pbr/Environment.hpp"
#include "Akila/graphics/gl/Texture.hpp"

#include "Akila/graphics/gl/Error.hpp"

#include "Akila/datas/Collection.hpp"

LoadingLayer::LoadingLayer(): Akila::Layer{} {
	Akila::Core::display->setTitle("camecasselescouilles");

	Akila::Core::resourcePool->setTexture("brdfLUT", Akila::Environment::createBRDFLUT());
	
	Akila::Core::resourcePool->setCubeMapTexture("skybox", std::make_shared<Akila::CubeMapTexture>(Akila::TextureBuffer::Format::RGB16F));
	Akila::Core::resourcePool->setCubeMapTexture("irradiance", std::make_shared<Akila::CubeMapTexture>(Akila::TextureBuffer::Format::RGB16F));
	Akila::Core::resourcePool->setCubeMapTexture("prefilter", std::make_shared<Akila::CubeMapTexture>(Akila::TextureBuffer::Format::RGB16F));

	Akila::Core::resourcePool->loadResourceFile("main.res", [this]() -> void {
		Akila::Core::resourcePool->getCubeMapTexture("skybox")->setSize(1024, 1024);
		Akila::Core::resourcePool->getCubeMapTexture("irradiance")->setSize(32, 32);
		Akila::Core::resourcePool->getCubeMapTexture("prefilter")->setSize(64, 64);

		Akila::Environment::createIBL(
			Akila::Core::resourcePool->getTexture("env"),
			Akila::Core::resourcePool->getCubeMapTexture("skybox"),
			Akila::Core::resourcePool->getCubeMapTexture("irradiance"),
			Akila::Core::resourcePool->getCubeMapTexture("prefilter"),
			Akila::Core::resourcePool->getMesh("invertedCube")
		);

		std::cout << "Fin chargement main.res" << std::endl;
		Akila::Core::layerManager->add(new PlayLayer{});
		Akila::Core::layerManager->remove(this);
	});

	defaultTriangle = Akila::Core::resourcePool->getMesh("akila_triangle");

	Akila::Core::display->getKeybord()->onKeyPress([](Akila::Keyboard::Key key) -> void {
		switch(key) {
			case Akila::Keyboard::Key::TAB:
				Akila::Core::display->setFullscreen(!Akila::Core::display->isFullscreen());
				break;

			case Akila::Keyboard::Key::ESC:
				Akila::Core::display->close();
				break;
		}
	});
}

void LoadingLayer::update() {}

void LoadingLayer::draw() {
	Akila::Core::renderer->useDefaultFrameBuffer();

	Akila::Core::renderer->disable(Akila::Renderer::DEPTH_TEST);
	Akila::Core::renderer->disable(Akila::Renderer::CULL_FACE);
	Akila::Core::renderer->render(Akila::Core::resourcePool->getMaterial("loadingScreen").get(), defaultTriangle.get());
}
