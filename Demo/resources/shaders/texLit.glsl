#akila_vertex

layout(location = a_position_loc) in vec4 a_position;
layout(location = a_uv_loc) in vec4 a_uv;

uniform mat4 PV;
uniform mat4 model;

out vec2 texCoord;

void main() {
	texCoord = a_uv.xy;
	gl_Position = PV * model * a_position;
}

#akila_fragment

in vec2 texCoord;
out vec4 fragColor;

uniform sampler2D diffuse;

void main() {
	vec3 color = texture(diffuse, texCoord).rgb;
	fragColor = vec4(color, 1.0);
}
