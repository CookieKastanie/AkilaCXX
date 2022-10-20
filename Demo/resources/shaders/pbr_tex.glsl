#akila_template shaders/templates/pbr_template.glsl

#akila_fragment

out vec4 FragColor;

uniform sampler2D diffuse;

void main() {
    FragColor = vec4(brdf(texture(diffuse, uv).rgb, 0.05, 1.0), 1.0);
}
