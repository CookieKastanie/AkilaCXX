#AKILA_VERTEX

out vec3 pos;
out vec3 normal;

uniform mat4 model;

void main() {
    vec4 viewPos = u_camera.view * model * a_position;
    pos = viewPos.xyz; 
    
    mat3 normalMatrix = transpose(inverse(mat3(u_camera.view * model)));
    normal = normalMatrix * a_normal.xyz;
    
    gl_Position = u_camera.projection * viewPos;
}

#AKILA_FRAGMENT

in vec3 pos;
in vec3 normal;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 fragPosition;
layout(location = 2) out vec4 fragNormal;

uniform vec3 color;

void main() {
    fragColor = vec4(color, 1.0);
    fragPosition = vec4(pos, 1.0);
    fragNormal = vec4(normalize(normal), 1.0);
}
