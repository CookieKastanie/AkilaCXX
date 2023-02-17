# Ressources

## Création de ressources

```cpp
Ref<MyResource> r1 = Resources::create<MyResource>("name1");
Ref<MyResource> r2 = Resources::create<MyResource>("name2", ConstructorArgs...);

//...

Ref<MyResource> r = Resources::get<MyResource>("name1");
```

## Charger des ressources depuis un JSON

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

## Dossiers par defaut

- RootFolder : `appdata`
- ResourcesFolder : `resources`
- UserDataFolder : `user`

## Loaders par defaut

- Texture2DLoader
- StaticMeshLoader
- ShaderSourceLoader
- MaterialLoader
- AudioLoader

Exemple :

```JSON
{
	"texture2d": [
		{
			"name": "samus",
			"path": "textures/samus_sprite.png",
			"format": "SRGB",
			"mips": false,
			"wrap_s": "CLAMP_TO_BORDER",
			"wrap_t": "CLAMP_TO_BORDER",
			"mag_filter": "NEAREST",
			"min_filter": "NEAREST",
			"border_color": [0, 0, 0, 0]
		}
	],

	"static_mesh": [
		{
			"name": "player",
			"path": "meshes/player.glb"
		},
		{
			// utiliser un prefix dans le cas ou
			// le fichier contient plusieurs modeles
			"prefix": "mob_", 
			"path": "meshes/enemies.glb"
		}
	],

	"shader_source": [
		{
			"name": "my_template",
			"path": "shaders/my_template.glsl"
		}
	],

	"material": [
		{
			"name": "sprite_mat",
			"source_path": "shaders/sprite_mat.glsl",
			"uniforms": {
				"color": [1.0, 1.0, 1.0]
			},
			"texture2d": {
				"albedo": "player"
			}
		}
	],

	"audio": [
		{
			"name": "jump",
			"path": "sounds/jump.mp3"
		}
	]
}
```
Le parser JSON accepte les commentaires.

## Enums des textures

### Format
```
DEPTH_COMPONENT
DEPTH_STENCIL

RED
RG
RGB
RGBA
SRGB
SRGB_ALPHA
RGB16F
RGBA16F

COMPRESSED_RED
COMPRESSED_RG
COMPRESSED_RGB
COMPRESSED_RGBA
COMPRESSED_SRGB
COMPRESSED_SRGB_ALPHA
```

### Wrap mode
```
REPEAT
CLAMP_TO_EDGE
CLAMP_TO_BORDER
MIRRORED_REPEAT
```

### Filter mode
```
LINEAR
NEAREST
NEAREST_MIPMAP_NEAREST
LINEAR_MIPMAP_NEAREST
NEAREST_MIPMAP_LINEAR
LINEAR_MIPMAP_LINEAR
```
