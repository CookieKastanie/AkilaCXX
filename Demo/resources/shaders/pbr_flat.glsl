#akila_template shaders/pbr_template.glsl

#akila_fragment

out vec4 FragColor;

uniform vec3 color = vec3(1.0, 0.0, 0.0);

void main() {
    FragColor = vec4(brdf(color, 0.4, 0.0), 1.0);
}
