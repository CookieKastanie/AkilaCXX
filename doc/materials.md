# Matériaux

## Ajouter des sources

```cpp
MaterialFactory::setSource("template_header", GLSL_TEMPLATE_HEADER);
MaterialFactory::setSource("camera_struct", GLSL_CAMERA_STRUCT);
MaterialFactory::setSource("my_template", "from literal or file");

MaterialFactory::define("u_camera_loc", CAMERA_LOCATION);

// les uniforms dit "réservés" sont innaccessible pour
// les instances de matériaux
MaterialFactory::reserveUniform("u_modelMatrix");
```

## Créer un matériaux

```cpp
Ref<Material> mat = MaterialFactory::build("mat_name", "source_name");
```

## Utilisation et instances

```cpp
Ref<Material> mat = //...

// modifie un uniform
mat.write("color", Vec3{1.f, 0.f, .5f});

// modifie l'affectation d'une texture
mat.affect("albedo", Resources::get<Texture2d>("my_texture"));

// envoie les uniforms et bind les textures
mat.send();

// envoie directement une valeur à un uniform réservé
mat.sendReserved("my_reserved_uniform", value);

// ...

// création d'un instance
MaterialInstance inst{mat};

// une instance est une vertion légère d'un matériaux,
// il permet d'override les uniforms du matériaux de base
inst.write("color", Vec3{0.f, 1.f, .2f});
inst.affect("albedo", Resources::get<Texture2d>("my_other_texture"));
inst.send();
```

## Création de sources

### Directives préprocesseur

```glsl
#akila_vertex // début du vertex shader
#akila_geometrie // début du goometrie shader
#akila_fragment // début du fragment shader

#akila_include <source_name> // insert une autre source 
#akila_template // déclare la source comme template
#akila_user_code // dans un template, indique la position du code utilisateur 
#akila_use_template <template_name> // permet d'utiliser un template
```

### Exemple de template

```glsl
#akila_template
#akila_vertex

#akila_include template_header

uniform mat4 u_modelMatrix;

#akila_include camera_struct
layout(std140, binding = u_camera_loc) uniform camera_ubo {
	Camera u_camera;
};

vec4 getPosition() {
	vec4 W = u_modelMatrix * a_position;

	v_worldPos = W.xyz;

	mat3 normalMatrix = transpose(inverse(mat3(u_modelMatrix)));
	v_normal = normalMatrix * a_normal.xyz;
	v_tangent = normalMatrix * a_tangent.xyz;

	v_texcoord_0 = a_texcoord_0.xy;
	v_texcoord_1 = a_texcoord_1.xy;
	v_texcoord_2 = a_texcoord_2.xy;

	v_color_0 = a_color_0;
	v_color_1 = a_color_1;
	v_color_2 = a_color_2;

	return u_camera.pv * W;
}

#akila_user_code

#akila_fragment

#akila_include template_header

#akila_include camera_struct
layout(std140, binding = u_camera_loc) uniform camera_ubo {
	Camera u_camera;
};

vec4 lit(vec3 color) {
	vec3 N = normalize(v_normal) * (gl_FrontFacing ? 1. : -1.);
	vec3 V = normalize(u_camera.position - v_worldPos);

	float d = 1.0 - dot(N, V);
	float frenel = d * d * 0.5;

	return vec4(color * 0.95 + vec3(frenel), 1.0);
}

#akila_user_code
```

### Exemple d'utilisation du template

```glsl
#akila_use_template my_template

#akila_vertex

void main() {
	gl_Position = getPosition();
}

#akila_fragment

out vec4 fragColor;

layout(binding = 0) uniform sampler2D albedo;

void main() {
	fragColor = lit(texture(albedo, v_texcoord_0).rgb);
}
```
