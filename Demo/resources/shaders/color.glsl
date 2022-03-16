#AKILA_VERTEX

void main() {
    gl_Position = u_camera.pv * a_position;
}

#AKILA_FRAGMENT

uniform vec3 color;

out vec4 FragColor;

void main(void) {
    FragColor = vec4(color, 1.0);
}
