#akila_vertex

out vec2 texCoord;
void main() {
    //vec2 position = vec2(gl_VertexID % 2, gl_VertexID / 2) * 4.0 - 1;
    //texCoord = (position + 1) * 0.5;
    //gl_Position = vec4(position, 0, 1);


    if(gl_VertexID == 0) {
        texCoord = vec2(0);
        gl_Position = vec4(-1, -1, 0, 1);
    }

    if(gl_VertexID == 1) {
        texCoord = vec2(1, 0);
        gl_Position = vec4(3, -1, 0, 1);
    }

    if(gl_VertexID == 2) {
        texCoord = vec2(0, 1);
        gl_Position = vec4(-1, 3, 0, 1);
    }
}

#akila_fragment

in vec2 texCoord;

out vec4 fragColor;

void main() {
	fragColor = vec4(texCoord, 1.0, 1.);
}
