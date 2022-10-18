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
- [Audio](#audio)
- [Coroutines](#coroutines)
## Code de départ
```cpp
#include <akila/akila.hpp>
using namespace akila;

class MyLayer: public Layer {
public:
	void tick() override {}
	void frame() override {}
	void gui() override {}
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
// indiquer dans quelle pile sera ajoutés les signaux
Signals::registerType<MySignalType>(Signals::Stack::BEFORE_TICK);

// écoute d'un type de signal
Listener listener = Signals::listen<MySignalType>([] (MySignalType const &e){
	//code
});

// emission d'un signal
Signals::emit<MySignalType>(Args...);
```
### Les Stacks
```
BEFORE_TICK
BEFORE_FRAME
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
Renderer::setClearColor(1.0f, 0.5f, 0.2f, 1.0f);
Renderer::enable(Renderer::Capability::DEPTH_TEST);
Renderer::clear();
```
### Buffers
```cpp
Ptr<VBO> positions = createPtr<VBO>(3, 0); // vec3 sur l'attribut 0
positions->setData(std::vector<Vec2>({
	{3, 1, 0}, {-1, 1, 0}, {-1, -3, 0}
}));

Ptr<VBO> uvs = createPtr<VBO>(2, 1); // vec2 sur l'attribut 1
uv->setData(std::vector<Vec2>({
	{2, 1}, {0, 1}, {0, -1}
}));

Ptr<VAO> vao = createPtr<VAO>();
vao->setDrawMode(VAO::Mode::POINTS); // TRIANGLES par defaut
vao->bind();
vao->registerVBO(positions.get());
vao->registerVBO(uvs.get());
vao->unbind();

//...

vao->draw();

```
### Textures
```cpp
Ref<Texture2D> tex = Ressources::create<Texture2D>(); // Texture2DArray, Cubemap, ...

tex->setSize(256, 256);

float data[] = {...};
unsigned int mipLvl = 0;

//            data contient un seul cannal...    de type float
tex->setData(&data, TextureBuffer::Format::RED, TextureBuffer::Type::FLOAT, mipLvl);

// changement des parametres de la texture
TextureBuffer::Parameters params;
params.magFilter = TextureBuffer::FilterMode::LINEAR;
params.minFilter = TextureBuffer::FilterMode::LINEAR;
params.wrapS = TextureBuffer::WrapMode::REPEAT;
params.wrapT = TextureBuffer::WrapMode::REPEAT;
tex->setParameters(params);
```
### Shaders
#### Instancier un shader
```cpp
// les shaders ont une bonne place dans resources (mais ce n'est pas obligatoire)
Ref<Shader> shader = Rersources::create<Shader>("#full_shader_code"); // passe par un preproc basique
// ou
Ref<Shader> shader = Rersources::create<Shader>("#vertex_shader", "#fragment_shader", "#geometry_shader");
```
#### Exemple de shader
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
#### Exemple avec un template

`path/unlit.glsl` qui sert de template :
```glsl
#akila_vertex

layout(location = a_position_loc) in vec4 a_position;
layout(location = a_uv_loc) in vec4 a_uv;

uniform mat4 PV;
uniform mat4 M;

out vec2 v_uv;

void main() {
	v_uv = a_uv.xy;
	glPosition = PV * M * a_position;
}

#akila_fragment

in vec2 v_uv;

out vec4 fragColor;

vec4 brdf(...) {
	fragColor = ...;
}

// macro qui indique ou sera inséré le code du fragment
#akila_user_code
```

Un shader qui utilise le template :
```glsl
#akila_template path/unlit.glsl

#akila_fragment

uniform sampler2D albedo;

void main() {
	vec3 color = texture(albedo, v_uv);

	brdf(...);
}

```
### Materiaux
```cpp
Material mat{shader}; // shader est de type Ref<Shader>

bool valid = mat.use("myUniform"); // indiquer que ce material utilisera "myUniform"
// la valeur de retour indique si cet uniform existe effectivement
// c'est chiant de devoir "déclarer" les uniforms que le material utilisera,
// mais l'objectif est de limiter la communication CPU/GPU

mat.write("myUniform", data); // écrit dans la mémoire du material

mat.affect("mySampler2D", Resources::get<Texture2D>("myTexture")); // ajoute une paire uniform / texture
// dans ce cas dans la shader, l'uniform doit être un sampler2D

mat.send(); // envoi au shader les data enregistrés dans le material
```
### FrameBuffers
```cpp
FrameBuffer framebuffer{};
framebuffer.setTexture(Resources::create<Texture2D>("colorTarget"), 0); // 0 = index d'attachment
framebuffer.setDepthTexture(Resources::create<Texture2D>("depthTarget", TextureBuffer::Format::DEPTH_COMPONENT));
framebuffer.resizeAll(Window::getSize());
framebuffer.prepare(); // appeler 1 fois pour commencer à render

//render...
framebuffer.bind(0);

//...

frambuffer.blitToDefault(0);
```
## Audio
(ça ne marche PAS DU TOUT LA 🤬)
```cpp
Ref<AudioSource> source = Resources::get<AudioSource>("mySource");

AudioEmitter emitter{source};

emitter.setPosition({1, 0, 5});

// jouer le sons :
emitter.play();
// ou
Audio::detath(emitter); // detach permet de lancer des instances, pour simultanément jouer le même son plusieurs fois
```

## Coroutines
```cpp
Coroutine co;

co.start([](Coroutine::Context const &ctx) {
	//...
	ctx.yield();
	//...
	ctx.wait(0.5f);
	//...
});

co.resume(Time::delta); // si dans frame
co.resume(); // Time::delta par defaut
co.resume(Time::fixedDelta); // si dans tick
co.resume(0.1f); // ou bien sur n'importe quel delta 
```