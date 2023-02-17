constexpr char * GLSL_TEMPLATE_HEADER = R""(
#akila_vertex

layout(location = a_position_loc) in vec4 a_position;
layout(location = a_normal_loc) in vec4 a_normal;
layout(location = a_tangent_loc) in vec4 a_tangent;

layout(location = a_texcoord_0_loc) in vec4 a_texcoord_0;
layout(location = a_texcoord_1_loc) in vec4 a_texcoord_1;
layout(location = a_texcoord_2_loc) in vec4 a_texcoord_2;

layout(location = a_color_0_loc) in vec4 a_color_0;
layout(location = a_color_1_loc) in vec4 a_color_1;
layout(location = a_color_2_loc) in vec4 a_color_2;

layout(location = a_joints_0_loc) in vec4 a_joints_0;
layout(location = a_joints_1_loc) in vec4 a_joints_1;
layout(location = a_joints_2_loc) in vec4 a_joints_2;

layout(location = a_weight_0_loc) in vec4 a_weight_0;
layout(location = a_weight_1_loc) in vec4 a_weight_1;
layout(location = a_weight_2_loc) in vec4 a_weight_2;

out vec3 v_worldPos;
out vec3 v_normal;
out vec3 v_tangent;

out vec2 v_texcoord_0;
out vec2 v_texcoord_1;
out vec2 v_texcoord_2;

out vec4 v_color_0;
out vec4 v_color_1;
out vec4 v_color_2;

#akila_fragment

in vec3 v_worldPos;
in vec3 v_normal;
in vec3 v_tangent;

in vec2 v_texcoord_0;
in vec2 v_texcoord_1;
in vec2 v_texcoord_2;

in vec4 v_color_0;
in vec4 v_color_1;
in vec4 v_color_2;
)"";
