# Akila
## Sommaire
 - [Code de départ](#code-de-départ)
 - [Fenêtre principale](#fenêtre-principale)
 - [ECS](#ecs)
	 - [Création d'une entité](#création-dune-entité)
	 - [Création d'un système](#création-dun-système)
 - [Signaux](#signaux)
	 - [Utilisation](#utilisation)
	 - [Les Stacks](#les-stacks)
	 - [Signaux intégrés](#signaux-intégrés)
 - [Gestion des ressources](#gestion-des-ressources)
	 - [Création de ressources](#création-de-ressources)
	 - [Charger des ressources depuis un JSON](#charger-des-ressources-depuis-un-json)
- [Threadpool](#threadpool)
- [Rendu](#rendu)
	- [Renderer](#renderer)
	- [Buffers](#buffers)
	- [Textures](#textures)
	- [Shaders](#shaders)
	- [Materiaux](#Materiaux)
	- [FrameBuffers](#framebuffers)
## Code de départ
```cpp
#include <akila/akila.hpp>
using namespace akila;

class MyLayer: public Layer {
public:
	void update() override {}
	void draw() override {}
	void drawImGui() override {}
}

int main() {
	return Core::run([]() {
		FileSystem::setRootFolder("resources");
		Layers::add<MyLayer>();
	});
}
```
## Fenêtre principale
```cpp
Window::setTitle("Title");

Window::setSize(IVec2{1280, 720});
IVec2 size = Window::getSize();

// ferme la fenêtre, et par extension, l'application
Window::close();
```
## ECS
### Création d'une entité
#### Basic
```cpp
Entity e = ECS::createEntity();
e.addComponent<MyStruct>();
e.addComponent<MyOtherStruct>({"Value"});
```
#### Par signature (plus performant)
```cpp
Signature s = ECS::createSignature<MyStruct, MyOtherStruct>();
Entity e = ECS::createEntity(s);
e.getComponent<MyOtherStruct>() = {"Value"};
```
### Création d'un système
L'attribut `entities` est hérité de la classe System. Il est automatiquement agrémenté des entités compatibles avec la signature du système.
```cpp
class MySystem: public System {
public:
	MySystem(): System{ECS::createSignature<MyStruct>()} {};

	void myMethode() {
		for(Entity e : entities) {
			auto &... = e.getComponent<...>();
			// logic here

			if(...) {
				ECS::addToEraseQueue(e);
			}
		}

		ECS::flushEraseQueue();
	}

	void onAdd(Entity entity) override {
		//...
	}

	void onRemove(Entity entity) override {
		//...
	}
}

//...

ECS::createSystem<MySystem>();

//...

MySystem *system = ECS::getSystem<MySystem>();
```
## Signaux
### Utilisation
```cpp
// création d'un nouveau type de signal
// indiquer dans quelle pile sera ajout� les signaux
Signals::registerType<MySignalType>(Signals::Stack::BEFORE_UPDATE);

// écoute d'un type de signal
Listener listener = Signals::listen<MySignalType>([] (MySignalType const &e){
	//code
});

// emission d'un signal
Signals::emit<MySignalType>(Args...);
```
### Les Stacks
```
BEFORE_UPDATE
BEFORE_DRAW
```
### Signaux intégrés
```
KeyPressSignal
KeyReleaseSignal
```
## Gestion des ressources
### Création de ressources
```cpp
Ref<MyResource> r1 = Resources::create<MyResource>("name1");
Ref<MyResource> r2 = Resources::create<MyResource>("name2", ConstructorArgs...);

//...

Ref<MyResource> r = Resources::get<MyResource>("name1");
```
### Charger des ressources depuis un JSON
```cpp
// Définition d'un Loader
class MyLoader: public Loader {
public:
	MyLoader(): Loader{"json_list_name"} {}
	void onEntry(JSON json, LoaderCallback cb) override {
		// utilisation du json...

		cb.success();
	}
}

Resources::registerLoader<MyLoader>();

//...

Resources::load("path/file.json", []() {
	// callback
});

Resources::load({"path/file1.json", "path/file2.json"}, []() {
	// callback
});
```
## Inputs
```cpp
if(Inputs::isPressed(Inputs::Key::A) {
	//...
}

Listener l = Signals::listen<KeyPressSignal>(KeyPressSignal const &s) {
	if(s.key == Inputs::Key::A) {
		//...
	}
}


Vec2 pos = Inputs::getMousePosition();
Vec2 vel = Inputs::getMouseVelocity();
```
## Threadpool
```cpp
Threadpool::submit([]() {
	// execution dans un thread
}, ()[] {
	// execution dans le thread principal
});
```
## Rendu
### Renderer
```cpp
Renderer::useDefaultFrameBuffer();
Renderer::setClearColor(1., .5, .2, 1.);
Renderer::enable(Renderer::Capability::DEPTH_TEST);
Renderer::clear();
```
### Buffers
```cpp

```
### Textures
```cpp

```
### Shaders
#### Instancier un shader
```cpp
// les shaders ont une bonne place dans resources (mais ce n'est pas obligatoire)
Ref<Shader> shader = Rersources::create<Shader>("#full_shader_code"); // passe par un preproc basique
// ou
Ref<Shader> shader = Rersources::create<Shader>("#vertex_shader", "#fragment_shader", "#geometry_shader");
```
#### Exemple de shader qui passe au preproc
```glsl
#akila_vertex

out vec2 texCoord;
void main() {
	texCoord.x = (gl_VertexID == 1) ? 2.0 : 0.0;
	texCoord.y = (gl_VertexID == 2) ? 2.0 : 0.0;

	gl_Position = vec4(texCoord * vec2(2.0) + vec2(-1.0), 1.0, 1.0);
}

#akila_fragment

// inclusion d'un autre fichier (n'est pas récursif cependant)
#akila_file path/myFile.glsl

in vec2 texCoord;

out vec4 fragColor;

void main() {
	fragColor = vec4(texCoord, 0.0, 1.0);
}
```
### Materiaux
```cpp
Material mat{shader}; // shader est de type Ref<Shader>

bool valid = mat.use("myUniform"); // indiquer que ce material utilisera "myUniform"
// la valeur de retour indique si cet uniform existe effectivement
// c'est chiant de devoir "déclarer" les uniforms que le material utilisera, mais l'objectif est de limiter la communication CPU/GPU

mat.write("myUniform", data); // écrit dans la mémoire du material

mat.send(); // envoi au shader les data enregistrés dans le material
```
### FrameBuffers
```cpp

```
----------------
----------------
----------------
----------------
----------------
----------------
----------------
----------------
----------------
----------------
----------------
----------------
----------------
----------------
----------------
----------------
----------------
----------------
Idées :
## Machine à états
```cpp
StateMachine m<MyStruct>{};
m.addState(Triggers::FRAME_START, 0, [](Index i, MyStruct &s){
	// logic
	return i + 1;
});
m.addStateThread(Triggers::FRAME_START, 1, [](Index i, MyStruct &s){
	// logic
	return i + 1;
});

if(!m.running()) m.start(0);
```
## Animateur
```cpp
Animator::???
```
