# Akila
## Code de départ
```cpp
#include <akila/akila.hpp>
using namespace akila;

int main() {
	return Core::run(???);
}
```
## ECS
### Création d'un composant
```cpp
struct MyStruct {...};

ECS::registerComponent<MyStruct>();
```
### Création d'un entité
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

...

ECS::registerSystem<MySystem>();

...

MySystem *system = ECS::getSystem<MySystem>();
```

### Gestion des ressources
```cpp
Resources::registerType<MyResource>();

// Loader pour chager depuis un JSON
class MyLoader: public Loader {
public:
	MyLoader(): Loader{"json_list_name"} {}
	void onEntry(json &const j) {}
}
Resources::registerType<MyResource, MyLoader>();
...

Ref<MyResource> r1 = Resources::create<MyResource>("name1");
Ref<MyResource> r2 = Resources::create<MyResource>("name2", ConstructorArgs...);

...

Ref<MyResource> r = Resources::get<MyResource>("name1");
```
#### Ressources depuis un JSON
```cpp
Resources::load("path/file.json", []() {
	// callback
});

Resources::load({"path/file1.json", "path/file2.json"}, []() {
	// callback
});
```
### Fenêtre principale
```cpp
Window::setTitle("Title");
Window::setSize(1280, 720);
```
```cpp

```
```cpp

```
```cpp

```
```cpp

```
