#AKILA_VERTEX

uniform float t;

void main() {
	gl_Position = u_camera.pv * mix(a_position, a_uv, t);
}

#AKILA_FRAGMENT
	
uniform vec3 color;
out vec4 fragColor;
	
void main() {
	fragColor = vec4(color, 1.0);
}
