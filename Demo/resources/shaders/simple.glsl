#akila_vertex

uniform mat4 PV;

out vec2 texCoord;
void main() {
	texCoord.x = (gl_VertexID == 1) ? 2.0 : 0.0;
	texCoord.y = (gl_VertexID == 2) ? 2.0 : 0.0;

	vec4 pos = vec4(texCoord * vec2(2.0) + vec2(-1.0), -1.5, 1.0);

	gl_Position = PV * pos;
}

#akila_fragment

#akila_file shaders/common.glsl

in vec2 texCoord;
out vec4 fragColor;

uniform float hue;
uniform sampler2D diffuse;

void main() {
	vec3 color = texture(diffuse, texCoord).rgb;

	color = rgb2hsv(color);
	color.x += hue;
	color = hsv2rgb(color);

	fragColor = vec4(color, 1.0);
}
