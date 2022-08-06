#akila_vertex

layout(location = a_position_loc) in vec4 a_position;

uniform mat4 PV;
uniform vec3 color;
uniform mat4 model;

out vec3 vBC;

void main() {
	vBC = vec3(0);
	vBC[gl_VertexID % 3] = 1.;

	gl_Position = PV * model * a_position;
}

#akila_fragment

uniform vec3 color;

in vec3 vBC;

out vec4 fragColor;

float edgeFactor(){
    vec3 d = fwidth(vBC);
    vec3 a3 = smoothstep(vec3(0.0), d * 1.8, vBC);
    return min(min(a3.x, a3.y), a3.z);
}

void main() {
	fragColor = vec4(color + 1. - edgeFactor(), 1.);
}
