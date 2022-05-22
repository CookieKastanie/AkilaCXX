# Akila
## Sommaire
 - [Code de départ](#code-de-départ)
 - [Fenêtre principale](#fenêtre-principale)
 - [ECS](#ecs)
	 - [Création d’une entité](#création-d’une-entité)
	 - [Création d'un système](#création-d'un-système)
 - [Signaux](#signaux)
	 - [Utilisation](#utilisation)
	 - [Les Stacks](#les-stacks)
	 - [Signaux intégrés](#signaux-integres)
 - [Gestion des ressources](#gestion-des-ressources)
	 - [Création de ressources](#création-de-ressources)
	 - [Charger des ressources depuis un JSON](#charger-des-ressources-depuis-un-JSON)
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
		FileSystem::setResourceFolder("resources");
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
// indiquer dans quelle pile sera ajouté les signaux
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
// Loader pour chager depuis un JSON
class MyLoader: public Loader {
public:
	MyLoader(): Loader{"json_list_name"} {}
	void onEntry(json &const j) {}
}
Resources::setLoader<MyResource, MyLoader>();

//...

Ref<MyResource> r1 = Resources::create<MyResource>("name1");
Ref<MyResource> r2 = Resources::create<MyResource>("name2", ConstructorArgs...);

//...

Ref<MyResource> r = Resources::get<MyResource>("name1");
```
### Charger des ressources depuis un JSON
```cpp
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


IVec2 pos = Inputs::getMousePosition();
Vec2 pos = Inputs::getMouseViewportPosition();
```
----------------
----------------
----------------
----------------
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
```cpp

```
```cpp

```
```cpp

```
