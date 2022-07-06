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

void main() {
	fragColor = vec4(texCoord, 0.0, 1.0);
}
