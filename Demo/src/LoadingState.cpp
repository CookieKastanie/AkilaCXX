#include "Demo/LoadingState.hpp"

#include "Akila/graphics/pbr/Environment.hpp"
#include "Akila/graphics/gl/Texture.hpp"


#include "Akila/graphics/gl/Error.hpp"


class EnvTask: public Akila::Task {
public:
	void onMain() override {
		Akila::Core::resourcesBucket->getCubeMapTexture("skybox")->setSize(1024, 1024);
		Akila::Core::resourcesBucket->getCubeMapTexture("irradiance")->setSize(32, 32);
		Akila::Core::resourcesBucket->getCubeMapTexture("prefilter")->setSize(64, 64);

		Akila::Environment::createIBL(
			Akila::Core::resourcesBucket->getTexture("env"),
			Akila::Core::resourcesBucket->getCubeMapTexture("skybox"),
			Akila::Core::resourcesBucket->getCubeMapTexture("irradiance"),
			Akila::Core::resourcesBucket->getCubeMapTexture("prefilter"),
			Akila::Core::resourcesBucket->getMesh("invertedCube")
		);
	}
};



LoadingState::LoadingState(): Akila::State{} {
	Akila::Core::display->setTitle("camecasselescouilles");

	Akila::Core::resourcesBucket->setTexture("brdfLUT", Akila::Environment::createBRDFLUT());

	
	Akila::Core::resourcesBucket->setCubeMapTexture("skybox", std::make_shared<Akila::CubeMapTexture>(Akila::TextureBuffer::Format::RGB16F));
	Akila::Core::resourcesBucket->setCubeMapTexture("irradiance", std::make_shared<Akila::CubeMapTexture>(Akila::TextureBuffer::Format::RGB16F));
	Akila::Core::resourcesBucket->setCubeMapTexture("prefilter", std::make_shared<Akila::CubeMapTexture>(Akila::TextureBuffer::Format::RGB16F));

	Akila::Core::resourcesBucket->loadResourceFile("main.res", []() -> void {
		Akila::Core::taskManager->submit(new EnvTask{});
		std::cout << "Fin chargement main.res" << std::endl;
	});

	defaultTriangle = Akila::Core::resourcesBucket->getMesh("akila_triangle");

	//Akila::Core::renderer->setSharedCamera(std::make_shared<Akila::PerspectiveCamera>());
	Akila::Core::renderer->setSharedCamera(std::make_shared<MouseCamera>(Akila::Core::display->getMouse()));

	Akila::Core::display->getKeybord()->onKeyPress([](Akila::Keyboard::Key key) -> void {
		switch(key) {
			case Akila::Keyboard::TAB:
				Akila::Core::display->setFullscreen(!Akila::Core::display->isFullscreen());
				break;

			case Akila::Keyboard::ESC:
				Akila::Core::display->close();
				break;
		}
	});
}

void LoadingState::update() {
	auto &cam = Akila::Core::renderer->getSharedCamera();

	//if(Akila::Core::display->getKeybord()->isPressed(Akila::Keyboard::Key::SPACE))
	//float x = std::cos(Akila::Time::now) * 3.f;
	//float z = std::sin(Akila::Time::now) * 3.f;

	//cam->setPosition({x, 0.5, z});

	cam->update();
}

void LoadingState::draw() {
	Akila::Core::renderer->useDefaultFrameBuffer();

	Akila::Core::renderer->disable(Akila::Renderer::DEPTH_TEST);
	Akila::Core::renderer->disable(Akila::Renderer::CULL_FACE);
	Akila::Core::renderer->render(Akila::Core::resourcesBucket->getMaterial("loadingScreen").get(), defaultTriangle.get());

	

	Akila::Core::renderer->enable(Akila::Renderer::DEPTH_TEST);
	
	Akila::Core::renderer->clearDepth();

	glDepthFunc(GL_LEQUAL);
	Akila::Core::renderer->render(
		Akila::Core::resourcesBucket->getMaterial("skybox").get(),
		Akila::Core::resourcesBucket->getMesh("invertedCube").get()
	);
	glDepthFunc(GL_LESS);

	//Akila::Core::renderer->render(Akila::Core::resourcesBucket->getMaterial("brdfLUT").get(), defaultTriangle.get());
	Akila::Core::renderer->enable(Akila::Renderer::CULL_FACE);

	Akila::Core::renderer->render(
		Akila::Core::resourcesBucket->getMaterial("sword").get(),
		Akila::Core::resourcesBucket->getMesh("sword").get()
	);
}
