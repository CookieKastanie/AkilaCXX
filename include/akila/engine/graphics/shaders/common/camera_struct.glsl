constexpr char * GLSL_CAMERA_STRUCT = R""(
struct Camera {
	mat4 projection;
	mat4 view;
	mat4 pv;
	vec3 position;
	float ratio;
};
)"";
