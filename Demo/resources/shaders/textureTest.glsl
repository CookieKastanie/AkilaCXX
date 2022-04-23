#AKILA_VERTEX

out vec2 uv;

void main() {
	gl_Position = u_camera.pv * a_position;
	uv = a_uv.xy;
}

#AKILA_FRAGMENT

in vec2 uv;
out vec4 fragColor;

uniform sampler2D tex;

void main() {
    fragColor = mix(texture(tex, uv), vec4(uv, 0, 1), 0.5);
}
