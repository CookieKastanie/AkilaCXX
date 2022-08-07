#akila_vertex

layout(location = a_position_loc) in vec4 a_position;

void main() {
	gl_Position = a_position;
}

#akila_geometry

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 PV;
uniform mat4 model;
 
out vec3 vBC;
 
void main() {
	mat4 W = PV * model;

	for(int i = 0; i < 3; ++i) {
		vBC = vec3(0);
		vBC[i] = 1.;
		gl_Position = W * gl_in[i].gl_Position;
		EmitVertex();
	}

	EndPrimitive();
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
