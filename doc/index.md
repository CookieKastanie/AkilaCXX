# Code de départ

## Exemple minimum

```cpp
#include <akila/akila.hpp>
using namespace akila;

class MyLayer: public Layer {
public:
	void onMount() override {}
	void onUnmount() override {}

	void tick() override {}
	void frame() override {}
	void gui() override {}
};

int main() {
	return Core::run([]() {
		Layers::add<MyLayer>();
	});
}
```

## Exemple mieux

```cpp
#include <akila/akila.hpp>
using namespace akila;

class MyLayer: public SceneLayer {
public:
	using Super = SceneLayer;

	void onMount() override {
		Super::onMount();

		cameraSystem = ECS::createSystem<FreeCameraSystem>();
		renderSystem = ECS::createSystem<Fast3dRenderSystem>();
		// ...
	}

	void tick() override {
		Super::tick();
		// ...
	}

private:
	// ...
};

int main(int argc, char *argv[]) {
	Window::InitValues init;
	init.samples = 4;
	init.size = {1280, 720};

	// modifie les valeurs de init
	Core::parseArguments(init, argc, argv);

	// force le titre de la fenetre
	init.title = "Akila Engine !";
	
	return Engine::run(init, []() {
		// dossier racine par defaut : appdata
		// dossier des resources par defaut : appdata/resources
		// dossier des données utilisateur par defaut : appdata/user

		Fast3dRenderSystem::injectShaderSources();

		Layers::add<DebugLayer>(Layer::Depth::FRONT);

		Resources::load("main.json", []() {
			Layers::add<MyLayer>();
		});
	});
}
```

> **⚠️ Important : l'API ne doit pas être utilisé en dehors du runtime de Core (démarré par Engine).**

## API

- [Fenêtre de l'application](./window.md)
- [Ressources](./resources.md)
- [Signaux](./signals.md)
- [Inputs](./inputs.md)
- [RHI](./rhi.md)
- [Materiaux](./materials.md)
- [Audio](./audio.md)
- [ECS](./ecs.md)
- [Coroutines](./coroutines.md)
- [Threadpool](./threadpool.md)
