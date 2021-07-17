# AkilaCTK

🍪 CookieKastanie 🍪

## Shaders

Vertex attributes :

	vec4 a_position;
	vec4 a_uv;
	vec4 a_normal;
	vec4 a_tangent;
	vec4 a_color;

Common structs :

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
	
Uniforms

	akila_camera u_camera;
	float u_time;
	akila_lights u_lights;

Default template :
	
	#AKILA_VERTEX
	
	out  vec2 uv;
	
	void main() {
		gl_Position = a_position;
		uv = a_uv.xy;
	}

	#AKILA_FRAGMENT
	
	in  vec2 uv;
	out  vec4 fragColor;
	
	void main() {
		fragColor = vec4(uv, 0.0, 1.0);
	}

## Resources files

