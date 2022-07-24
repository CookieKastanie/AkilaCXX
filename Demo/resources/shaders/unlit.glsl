#akila_vertex

layout(location = a_position_loc) in vec4 a_position;
//layout(location = a_uv_loc) in vec4 a_uv;

uniform mat4 PV;

//out vec2 uv;

uniform vec3 color;
uniform mat4 model;

void main() {
	//uv = a_uv.xy;

	gl_Position = PV * model * a_position;
}

#akila_fragment

//in vec2 uv;

uniform vec3 color;

out vec4 fragColor;

void main() {
	//fragColor = vec4(uv, 0., 1.);
	fragColor = vec4(color, 1.);
}
