#include "utils_layer.hpp"
#include "test_layer.hpp"
#include "rat_layer.hpp"
#include "kitchen_layer.hpp"
#include "coro_layer.hpp"

using namespace akila;

UtilsLayer::UtilsLayer(): isLoading{false} {
	keyListener = Signals::listen<KeyPressSignal>([&](KeyPressSignal const &keySignal) {
		switch(keySignal.key) {
			case Inputs::Key::ESC:
				Window::close();
				break;

			case Inputs::Key::TAB:
				Window::setFullscreen(!Window::isFullscreen());
				break;

			//*/
			case Inputs::Key::NUM_1:
					
					if(isLoading) return;

					isLoading = true;
					Layers::remove<RatLayer>();
					Layers::remove<TestLayer>();
					//ECS::resetAll();
					//Resources::cleanAll();

					Resources::load({"main.json"}, [&]() {
						Layers::add<TestLayer>();
						isLoading = false;
					});
				break;
				/*/
				if(isLoading) break;

				isLoading = true;
				Layers::remove<RatLayer>();
				Layers::remove<TestLayer>();
				ECS::resetAll();
				Resources::cleanAll();

				Resources::load({"main.json"}, [&]() {
					Layers::add<TestLayer>();
					isLoading = false;
				});
				break;
				//*/
			case Inputs::Key::NUM_2:
				Threadpool::submit([]() {}, [&]() {

					if(isLoading) return;

					isLoading = true;
					Layers::remove<RatLayer>();
					Layers::remove<TestLayer>();
					//ECS::resetAll();
					//Resources::cleanAll();

					Resources::load({"rat.json"}, [&]() {
						Layers::add<RatLayer>();
						isLoading = false;
					});
				});
				break;
				/*/
				if(isLoading) break;

				isLoading = true;
				Layers::remove<RatLayer>();
				Layers::remove<TestLayer>();
				ECS::resetAll();
				Resources::cleanAll();

				Resources::load({"rat.json"}, [&]() {
					Layers::add<RatLayer>();
					isLoading = false;
				});
				break;

				//*/
		}
	});

	/*/
	Resources::load({"main.json"}, []() {
		std::cout << "Loaded" << std::endl;
		Layers::add<TestLayer>();
	});
	//*/


	/*/
	Resources::load({"rat.json"}, []() {
		Layers::add<RatLayer>();
	});
	//*/

	/*/
	Layers::add<KitchenLayer>();
	//*/

	//*/
	Layers::add<CoroLayer>();
	//*/
}

void UtilsLayer::tick() {
	/*/
	if(Inputs::isPressed(Inputs::Key::NUM_1)) {
		if(isLoading) return;

		isLoading = true;
		Layers::remove<RatLayer>();
		Layers::remove<TestLayer>();
		ECS::resetAll();
		Resources::cleanAll();

		Resources::load({"main.json"}, [&]() {
			Layers::add<TestLayer>();
			isLoading = false;
			});
		return;
	}

	if(Inputs::isPressed(Inputs::Key::NUM_1)) {
		if(isLoading) return;

		isLoading = true;
		Layers::remove<RatLayer>();
		Layers::remove<TestLayer>();
		ECS::resetAll();
		Resources::cleanAll();

		Resources::load({"rat.json"}, [&]() {
			Layers::add<RatLayer>();
			isLoading = false;
			});
		return;
	}
	//*/
	
}
void UtilsLayer::frame() {}
void UtilsLayer::gui() {}
