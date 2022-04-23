# AkilaCTK

🍪 CookieKastanie 🍪

## Starting code

```cpp
#include "Akila/core/Core.hpp"

int main(int argc, char *argv[]) {
	return Akila::Core::run(argc, argv, []() {
		Akila::FileSystem::setResourceFolder("resources"); // path relative to the executable
		Akila::Core::layerManager->add(new MyLayer{}); // custom class who extend Akila::Layer
	});
}
```

## Shaders

Integrated vertex attributes :

```glsl
vec4 a_position;
vec4 a_uv;
vec4 a_normal;
vec4 a_tangent;
vec4 a_color;
```

Integrated common structs :

```glsl
struct akila_camera {
	mat4 projection;
	mat4 view;
	mat4 pv;
	vec3 position;
	float ratio;
};
	
struct akila_lights {
	vec3 directionalLight;
	vec3 directionalColor;
	vec3 pointsPositions[AKILA_POINT_LIGHT_COUNT];
	vec3 pointsColors[AKILA_POINT_LIGHT_COUNT];
};
```

Integrated uniforms :

```glsl
akila_camera u_camera;
float u_time;
akila_lights u_lights;
```

Basic template :

```glsl
#AKILA_VERTEX
	
out vec2 uv;
	
void main() {
	gl_Position = a_position;
	uv = a_uv.xy;
}

#AKILA_FRAGMENT
	
in vec2 uv;
out vec4 fragColor;
	
void main() {
	fragColor = vec4(uv, 0.0, 1.0);
}
```

## Resource files

Objects :

```json
{
	"shaders": [
		{
			"name": "string",
			"src": "string",

			"uniforms-i": {
				"name1": [ints, ...]
				"name2": int
			},

			"uniforms-f": {
				"name3": [floats, ...]
				"name4": float
			}
		}
	],

	"textures": [
		{
			"name": "string",
			"src": "string",

			"format": "stored texture format"

			"minFilter": "filter texture mode"
			"magFilter": "filter texture mode"

			"wrapS": "wrap texture mode"
			"wrapT": "wrap texture mode"
			"wrapR": "wrap texture mode"

			"mips": "boolean"
		},
	],

	"meshs": [
		{
			"name": "string",
			"src": "string"
		}
	],

	"materials": [
		{
			"name": "string",
			"shader": "string",

			"texture": {
				"textName": unit
			},

			"cubemap": {
				"cubemapName": unit
			},

			"uniforms-i": {
				"name1": [ints, ...]
				"name2": int
			},

			"uniforms-f": {
				"name3": [floats, ...]
				"name4": float
			}
		}
	]
}
```

Texture formats :

```css
RED
RG
RGB
RGBA
SRGB
SRGB_ALPHA
RGB16F
RGBA16F
DEPTH_COMPONENT
DEPTH_STENCIL
```

Texture filters :

```css
NEAREST
LINEAR
NEAREST_MIPMAP_NEAREST
LINEAR_MIPMAP_NEAREST
NEAREST_MIPMAP_LINEAR
LINEAR_MIPMAP_LINEAR
```

Texture wraps :

```css
CLAMP_TO_EDGE
CLAMP_TO_BORDER
MIRRORED_REPEAT
REPEAT
```

## Load a resource file

```cpp
Akila::Core::resourcePool->load("myResourceFile.json", []() {
	std::cout << "Done !" << std::endl;
});
```
