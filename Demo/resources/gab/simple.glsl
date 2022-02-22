#AKILA_VERTEX

uniform mat4 model;

void main() {
    gl_Position = u_camera.pv * model * a_position;
}

#AKILA_FRAGMENT

out vec4 fragColor;

uniform vec3 color;

void main() {
    fragColor = vec4(color, 1.0);
}
