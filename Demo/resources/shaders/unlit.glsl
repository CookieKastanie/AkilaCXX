#akila_vertex

layout(location = a_position_loc) in vec4 a_position;
//layout(location = a_normal_loc) in vec4 a_normal;

uniform mat4 PV;

void main() {
	gl_Position = PV * a_position;
}

#akila_fragment

out vec4 fragColor;

void main() {
	fragColor = vec4(1., .2, .2, 1.);
}
