constexpr char const * GLSL_DITHER = R""(
const float[4] D2_PATTERN = float [](
	0.0,  0.5,
	0.75, 0.25
);

const float[16] D4_PATTERN = float [](
	0.0,    0.5,    0.125,  0.625,
	0.75,   0.25,   0.875,  0.375,
	0.1875, 0.6875, 0.0625, 0.5625,
	0.9375, 0.4375, 0.8125, 0.3125
);

const float[64] D8_PATTERN = float [](
	0.0,      0.5,      0.125,    0.625,    0.03125,  0.53125,  0.15625,  0.65625,
	0.75,     0.25,     0.875,    0.375,    0.78125,  0.28125,  0.90625,  0.40625,
	0.1875,   0.6875,   0.0625,   0.5625,   0.21875,  0.71875,  0.09375,  0.59375,
	0.9375,   0.4375,   0.8125,   0.3125,   0.96875,  0.46875,  0.84375,  0.34375,
	0.046875, 0.546875, 0.171875, 0.671875, 0.015625, 0.515625, 0.140625, 0.640625,
	0.796875, 0.296875, 0.921875, 0.421875, 0.765625, 0.265625, 0.890625, 0.390625,
	0.234375, 0.734375, 0.109375, 0.609375, 0.203125, 0.703125, 0.078125, 0.578125,
	0.984375, 0.484375, 0.859375, 0.359375, 0.953125, 0.453125, 0.828125, 0.328125
);

#define DITHER_TEMPLATE(type) \
type dither2(type color, float strength, vec2 coords) { \
	ivec2 st = ivec2(coords); \
	st.x = st.x % 2; \
	st.y = st.y % 2; \
	return color + strength * (D2_PATTERN[st.x + st.y * 2] - 0.5); \
} \
\
type dither2(type color, float strength) { \
	return dither2(color, strength, gl_FragCoord.xy); \
} \
\
type dither2(type color) { \
	return dither2(color, 1.0, gl_FragCoord.xy); \
} \
\
type dither4(type color, float strength, vec2 coords) { \
	ivec2 st = ivec2(coords); \
	st.x = st.x % 4; \
	st.y = st.y % 4; \
	return color + strength * (D4_PATTERN[st.x + st.y * 4] - 0.5); \
} \
\
type dither4(type color, float strength) { \
	return dither4(color, strength, gl_FragCoord.xy); \
} \
\
type dither4(type color) { \
	return dither4(color, 1.0, gl_FragCoord.xy); \
} \
\
type dither8(type color, float strength, vec2 coords) { \
	ivec2 st = ivec2(coords); \
	st.x = st.x % 8; \
	st.y = st.y % 8; \
	return color + strength * (D8_PATTERN[st.x + st.y * 8] - 0.5); \
} \
\
type dither8(type color, float strength) { \
	return dither8(color, strength, gl_FragCoord.xy); \
} \
\
type dither8(type color) { \
	return dither8(color, 1.0, gl_FragCoord.xy); \
} \
\
type quantify2(type color) { \
	return step(type(0.5), color); \
}

DITHER_TEMPLATE(float)
DITHER_TEMPLATE(vec2)
DITHER_TEMPLATE(vec3)

float quantify(float color, float res) {
	return floor(color * res) / (res - 1.);
}

vec3 quantify(vec3 color, float res) {
	return floor(color * res) / (res - 1.);
}

vec3 quantify(vec3 color, vec3 res) {
	return floor(color * res) / (res - 1.);
}
)"";
