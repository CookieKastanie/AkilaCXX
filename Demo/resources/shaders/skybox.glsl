#akila_vertex

layout(location = a_position_loc) in vec4 a_position;

struct Camera {
	mat4 projection;
	mat4 view;
	mat4 pv;
	vec3 position;
	float ratio;
};

layout(std140, binding = u_camera_loc) uniform camera_ubo {
    Camera u_camera;
};

out vec3 direction;

void main() {
	direction = a_position.xyz;
    gl_Position = vec4(u_camera.projection * vec4(mat3(u_camera.view) * direction, 1.)).xyzz;
}

#akila_fragment

uniform samplerCube cubemapTexture;

in vec3 direction;

out vec4 FragColor;

uniform float exposure = 1.;
uniform float gamma = 2.2;

void main(void) {
    vec3 color = texture(cubemapTexture, direction).rgb;
    color = vec3(1.0) - exp(-color * exposure);
    color = pow(color, vec3(1.0 / gamma));
    FragColor = vec4(color, 1.0);
}
