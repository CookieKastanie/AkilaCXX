constexpr char * GLSL_FAST3D_TEMPLATE = R""(
#akila_template
#akila_vertex

layout(location = a_position_loc) in vec4 a_position;
layout(location = a_normal_loc) in vec4 a_normal;
layout(location = a_texcoord_0_loc) in vec4 a_texcoord_0;

out vec3 v_normal;
out vec2 v_texcoord_0;
out vec3 v_worldPos;

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

	v_texcoord_0 = a_texcoord_0.xy;

	return u_camera.pv * W;
}

#akila_user_code

#akila_fragment

in vec3 v_normal;
in vec2 v_texcoord_0;
in vec3 v_worldPos;

#akila_include camera_struct
layout(std140, binding = u_camera_loc) uniform camera_ubo {
	Camera u_camera;
};

vec4 lit(vec3 color) {
	vec3 N = normalize(v_normal);
	vec3 V = normalize(u_camera.position - v_worldPos);

	float d = 1.0 - dot(N, V);
	float frenel = d * d * 0.5;

	return vec4(color * 0.95 + vec3(frenel), 1.0);
}

#akila_user_code
)"";
