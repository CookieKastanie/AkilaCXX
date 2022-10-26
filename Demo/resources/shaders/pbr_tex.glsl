#akila_template shaders/templates/pbr_template.glsl

#akila_fragment

out vec4 FragColor;

uniform sampler2D diffuse;
uniform float roughness = 0.6;
uniform float metallic = 0.0;

void main() {
    FragColor = vec4(brdf(texture(diffuse, uv).rgb, roughness, metallic), 1.0);
}
