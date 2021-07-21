#AKILA_VERTEX

out vec2 uv;

void main() {
	gl_Position = a_position;
	uv = a_uv.xy;
}

#AKILA_FRAGMENT

in vec2 uv;
out vec4 fragColor;

uniform vec3 color;

mat2 rotate(float a) {
    float c = cos(a);
    float s = sin(a);
    return mat2(
        c, -s,
        s,  c
    );
}

float rand(in float x) {
    return fract(sin(x * 12.9898) * 43758.5453123);
}

void main() {
    fragColor = vec4(0., 0., 0., 1.);

    // coords centrees
    vec2 cuv = uv * 2. - 1.;
    cuv.x *= u_camera.ratio;

    //distance depuis le centre
    float l = length(cuv);

    // nombre de cercle (echelle)
    const float n = 20.;
    // id du cercle
    float id = floor(l * n) / n;

    // ne pas afficher le plus petit cercle
    if(id < 0.05) return;

    // rotation du plan
    cuv *= rotate(rand(id) * PI * 2.);
    // conversion en coords polaires
    vec2 plr = vec2(l, atan(-cuv.y, -cuv.x));
    // passer de -PI..PI a 0..1
    plr.y = (plr.y + PI) / (PI * 2.);

    // animation cercle
    float t = sin(u_time * rand(id) * 1.25) * 0.5 + 0.5;
    t = mix(.48, .02, t);

    float value =
    smoothstep(.3, .9, sin(plr.x * PI * 2. * n)) * // rayon du cercle
    smoothstep(t - 0.001, t + 0.001, abs(plr.y - 0.5)); // longueur de la corde (en miroir avec le abs)

    // animation couleur
    //const vec3 baseColor = vec3(1.5, 2.9, 3.5);
    fragColor.rgb = (sin(color + id * 5. + u_time) * 0.5 + 0.5) * value;
}