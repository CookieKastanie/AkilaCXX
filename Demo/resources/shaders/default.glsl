#AKILA_VERTEX

out vec2 uv;

void main() {
	gl_Position = a_position;
	uv = a_uv.xy;
}

#AKILA_FRAGMENT

in vec2 uv;
out vec4 fragColor;

void main() {
	fragColor = vec4(uv, u_camera.position.x, 1.);
}
