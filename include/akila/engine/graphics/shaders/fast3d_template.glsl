constexpr char const * GLSL_FAST3D_TEMPLATE = R""(
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
)"";
