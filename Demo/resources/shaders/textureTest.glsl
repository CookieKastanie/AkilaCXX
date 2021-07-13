#AKILA_VERTEX

out vec2 uv;

void main() {
	gl_Position = a_position;
	uv = a_uv.xy;
}

#AKILA_FRAGMENT

in vec2 uv;
out vec4 fragColor;

uniform sampler2D tex;

void main() {
    fragColor = texture(tex, uv);
}
