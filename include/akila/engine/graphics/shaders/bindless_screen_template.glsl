constexpr char const * GLSL_BINDLESS_SCREEN_TEMPLATE = R""(
#akila_template
#akila_vertex

out vec2 v_texcoord;

void main() {
	v_texcoord.x = (gl_VertexID == 1) ? 2.0 : 0.0;
	v_texcoord.y = (gl_VertexID == 2) ? 2.0 : 0.0;

	gl_Position = vec4(v_texcoord * vec2(2.0) + vec2(-1.0), 1.0, 1.0);
}

#akila_fragment

in vec2 v_texcoord;

#akila_user_code
)"";
