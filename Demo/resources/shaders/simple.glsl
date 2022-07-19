#akila_vertex

#akila_file shaders/constants.glsl

out vec2 texCoord;
void main() {
	texCoord.x = (gl_VertexID == 1) ? 2.0 : 0.0;
	texCoord.y = (gl_VertexID == 2) ? 2.0 : 0.0;
 
	gl_Position = vec4(texCoord * vec2(2.0) + vec2(-1.0), 1.0, 1.0);
}

#akila_fragment

in vec2 texCoord;

out vec4 fragColor;

struct test {
	float varFloat;
	sampler2D varSampler;
};

uniform float blue;
uniform float green;

uniform sampler2D diffuse;

uniform test varTest;

uniform test arrFloat[5];


void main() {
	vec3 tex = texture(diffuse, texCoord).rgb;

	fragColor = vec4(
		vec3(texCoord.x, green, blue + arrFloat[3].varFloat * varTest.varFloat)
		+ tex * 0.5, 1.0);
}
