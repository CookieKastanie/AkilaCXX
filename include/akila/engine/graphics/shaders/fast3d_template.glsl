constexpr char * GLSL_FAST3D_TEMPLATE = R""(
#akila_template
#akila_vertex

layout(location = a_position_loc) in vec4 a_position;
layout(location = a_normal_loc) in vec4 a_normal;
layout(location = a_tangent_loc) in vec4 a_tangent;

layout(location = a_texcoord_0_loc) in vec4 a_texcoord_0;
layout(location = a_texcoord_1_loc) in vec4 a_texcoord_1;
layout(location = a_texcoord_2_loc) in vec4 a_texcoord_2;

layout(location = a_color_0_loc) in vec4 a_color_0;
layout(location = a_color_1_loc) in vec4 a_color_1;
layout(location = a_color_2_loc) in vec4 a_color_2;

out vec3 v_worldPos;
out vec3 v_normal;
out vec3 v_tangent;

out vec2 v_texcoord_0;
out vec2 v_texcoord_1;
out vec2 v_texcoord_2;

out vec4 v_color_0;
out vec4 v_color_1;
out vec4 v_color_2;

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

in vec3 v_worldPos;
in vec3 v_normal;
in vec3 v_tangent;

in vec2 v_texcoord_0;
in vec2 v_texcoord_1;
in vec2 v_texcoord_2;

in vec4 v_color_0;
in vec4 v_color_1;
in vec4 v_color_2;

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
