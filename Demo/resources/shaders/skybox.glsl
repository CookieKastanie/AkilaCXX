#AKILA_VERTEX

out vec3 direction;

void main() {
	direction = a_position.xyz;
    gl_Position = vec4(u_camera.projection * vec4(mat3(u_camera.view) * direction, 1.)).xyzz;
}

#AKILA_FRAGMENT

uniform samplerCube cubemapTexture;

in vec3 direction;

out vec4 FragColor;

uniform float exposure;
uniform float gamma;

void main(void) {
    vec3 color = texture(cubemapTexture, direction).rgb;
    color = vec3(1.0) - exp(-color * exposure);
    color = pow(color, vec3(1.0 / gamma));
    FragColor = vec4(color, 1.0);
}
