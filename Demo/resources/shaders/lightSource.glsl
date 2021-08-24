#AKILA_VERTEX

const mat4 M = mat4(
    vec4(.2,  0,  0, 0),
    vec4( 0, .2,  0, 0),
    vec4( 0,  0, .2, 0),
    vec4(-5,  5,  5, 1)
);

void main() {
    gl_Position = u_camera.pv * M * a_position;
}

#AKILA_FRAGMENT

uniform vec3 color;

out vec4 FragColor;

void main(void) {
    FragColor = vec4(color, 1.0);
}
