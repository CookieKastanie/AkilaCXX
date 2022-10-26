#akila_vertex

layout(location = a_position_loc) in vec4 a_position;

uniform mat4 projection;
uniform mat4 view;

out vec3 localPos;

void main() {
	localPos = a_position.xyz;  
	gl_Position = projection * view * a_position;
}

#akila_fragment

out vec4 fragColor;
in vec3 localPos;

uniform sampler2D equirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 sampleSphericalMap(vec3 v) {
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	uv *= invAtan;
	uv += 0.5;

	return uv;
}

void main() {	
	vec2 uv = sampleSphericalMap(normalize(localPos));
	vec3 color = texture(equirectangularMap, uv).rgb;

	fragColor = vec4(color, 1.0);
}
