#include "akila/engine/math_extension/easing.hpp"
#include "akila/core/math/math.hpp"

using namespace akila;

float Easing::linear(float start, float end, float value) {
	return mix(start, end, value);
}

float Easing::spring(float start, float end, float value) {
	value = clamp(value, 0.f, 1.f);
	value = (sin(value * PI * (0.2f + 2.5f * value * value * value)) * pow(1.f - value, 2.2f) + value) * (1.f + (1.2f * (1.f - value)));
	return start + (end - start) * value;
}

float Easing::easeInQuad(float start, float end, float value) {
	end -= start;
	return end * value * value + start;
}

float Easing::easeOutQuad(float start, float end, float value) {
	end -= start;
	return -end * value * (value - 2.f) + start;
}

float Easing::easeInOutQuad(float start, float end, float value) {
	value /= .5f;
	end -= start;
	if(value < 1.f) return end * 0.5f * value * value + start;
	value--;
	return -end * 0.5f * (value * (value - 2.f) - 1.f) + start;
}

float Easing::easeInCubic(float start, float end, float value) {
	end -= start;
	return end * value * value * value + start;
}

float Easing::easeOutCubic(float start, float end, float value) {
	value--;
	end -= start;
	return end * (value * value * value + 1.f) + start;
}

float Easing::easeInOutCubic(float start, float end, float value) {
	value /= .5f;
	end -= start;
	if(value < 1.f) return end * 0.5f * value * value * value + start;
	value -= 2.f;
	return end * 0.5f * (value * value * value + 2.f) + start;
}

float Easing::easeInQuart(float start, float end, float value) {
	end -= start;
	return end * value * value * value * value + start;
}

float Easing::easeOutQuart(float start, float end, float value) {
	value--;
	end -= start;
	return -end * (value * value * value * value - 1.f) + start;
}

float Easing::easeInOutQuart(float start, float end, float value) {
	value /= .5f;
	end -= start;
	if(value < 1.f) return end * 0.5f * value * value * value * value + start;
	value -= 2.f;
	return -end * 0.5f * (value * value * value * value - 2.f) + start;
}

float Easing::easeInQuint(float start, float end, float value) {
	end -= start;
	return end * value * value * value * value * value + start;
}

float Easing::easeOutQuint(float start, float end, float value) {
	value--;
	end -= start;
	return end * (value * value * value * value * value + 1.f) + start;
}

float Easing::easeInOutQuint(float start, float end, float value) {
	value /= .5f;
	end -= start;
	if(value < 1.f) return end * 0.5f * value * value * value * value * value + start;
	value -= 2.f;
	return end * 0.5f * (value * value * value * value * value + 2.f) + start;
}

float Easing::easeInSine(float start, float end, float value) {
	end -= start;
	return -end * cos(value * (PI * 0.5f)) + end + start;
}

float Easing::easeOutSine(float start, float end, float value) {
	end -= start;
	return end * sin(value * (PI * 0.5f)) + start;
}

float Easing::easeInOutSine(float start, float end, float value) {
	end -= start;
	return -end * 0.5f * (cos(PI * value) - 1.f) + start;
}

float Easing::easeInExpo(float start, float end, float value) {
	end -= start;
	return end * pow(2.f, 10.f * (value - 1.f)) + start;
}

float Easing::easeOutExpo(float start, float end, float value) {
end -= start;
return end * (-pow(2.f, -10.f * value) + 1.f) + start;
}

float Easing::easeInOutExpo(float start, float end, float value) {
	value /= .5f;
	end -= start;
	if(value < 1.f) return end * 0.5f * pow(2.f, 10.f * (value - 1.f)) + start;
	value--;
	return end * 0.5f * (-pow(2.f, -10.f * value) + 2.f) + start;
}

float Easing::easeInCirc(float start, float end, float value) {
	end -= start;
	return -end * (sqrt(1.f - value * value) - 1.f) + start;
}

float Easing::easeOutCirc(float start, float end, float value) {
	value--;
	end -= start;
	return end * sqrt(1.f - value * value) + start;
}

float Easing::easeInOutCirc(float start, float end, float value) {
	value /= .5f;
	end -= start;
	if(value < 1.f) return -end * 0.5f * (sqrt(1.f - value * value) - 1.f) + start;
	value -= 2.f;
	return end * 0.5f * (sqrt(1.f - value * value) + 1.f) + start;
}

float Easing::easeInBounce(float start, float end, float value) {
	end -= start;
	float d = 1.f;
	return end - easeOutBounce(0.f, end, d - value) + start;
}

float Easing::easeOutBounce(float start, float end, float value) {
	value /= 1.f;
	end -= start;
	if(value < (1.f / 2.75f)) {
		return end * (7.5625f * value * value) + start;
	} else if(value < (2.f / 2.75f)) {
		value -= (1.5f / 2.75f);
		return end * (7.5625f * (value)*value + .75f) + start;
	} else if(value < (2.5 / 2.75)) {
		value -= (2.25f / 2.75f);
		return end * (7.5625f * (value)*value + .9375f) + start;
	} else {
		value -= (2.625f / 2.75f);
		return end * (7.5625f * (value)*value + .984375f) + start;
	}
}

float Easing::easeInOutBounce(float start, float end, float value) {
	end -= start;
	float d = 1.f;
	if(value < d * 0.5f) return easeInBounce(0.f, end, value * 2.f) * 0.5f + start;
	else return easeOutBounce(0, end, value * 2.f - d) * 0.5f + end * 0.5f + start;
}

float Easing::easeInBack(float start, float end, float value) {
	end -= start;
	value /= 1.f;
	float s = 1.70158f;
	return end * (value)*value * ((s + 1.f) * value - s) + start;
}

float Easing::easeOutBack(float start, float end, float value) {
	float s = 1.70158f;
	end -= start;
	value = (value) - 1.f;
	return end * ((value)*value * ((s + 1.f) * value + s) + 1.f) + start;
}

float Easing::easeInOutBack(float start, float end, float value) {
	float s = 1.70158f;
	end -= start;
	value /= .5f;
	if((value) < 1.f) {
		s *= (1.525f);
		return end * 0.5f * (value * value * (((s)+1.f) * value - s)) + start;
	}
	value -= 2;
	s *= (1.525f);
	return end * 0.5f * ((value)*value * (((s)+1) * value + s) + 2) + start;
}

float Easing::easeInElastic(float start, float end, float value) {
	end -= start;

	float d = 1.f;
	float p = d * .3f;
	float s;
	float a = 0.f;

	if(value == 0.f) return start;

	if((value /= d) == 1.f) return start + end;

	if(a == 0.f || a < abs(end)) {
	a = end;
	s = p / 4.f;
	} else {
	s = p / (2.f * PI) * asin(end / a);
	}

	return -(a * pow(2.f, 10.f * (value -= 1.f)) * sin((value * d - s) * (2.f * PI) / p)) + start;
}

float Easing::easeOutElastic(float start, float end, float value) {
	end -= start;

	float d = 1.f;
	float p = d * .3f;
	float s;
	float a = 0.f;

	if(value == 0.f) return start;

	if((value /= d) == 1.f) return start + end;

	if(a == 0.f || a < abs(end)) {
		a = end;
		s = p * 0.25f;
	} else {
		s = p / (2.f * PI) * asin(end / a);
	}

	return (a * pow(2.f, -10.f * value) * sin((value * d - s) * (2.f * PI) / p) + end + start);
}

float Easing::easeInOutElastic(float start, float end, float value) {
	end -= start;

	float d = 1.f;
	float p = d * .3f;
	float s;
	float a = 0.f;

	if(value == 0) return start;

	if((value /= d * 0.5f) == 2.f) return start + end;

	if(a == 0.f || a < abs(end)) {
		a = end;
		s = p / 4.f;
	} else {
		s = p / (2.f * PI) * asin(end / a);
	}

	if(value < 1.f) return -0.5f * (a * pow(2.f, 10.f * (value -= 1.f)) * sin((value * d - s) * (2.f * PI) / p)) + start;
	return a * pow(2.f, -10.f * (value -= 1.f)) * sin((value * d - s) * (2.f * PI) / p) * 0.5f + end + start;
}

float Easing::linearD(float start, float end, float value) {
	(void)value;

	return end - start;
}

float Easing::springD(float start, float end, float value) {
	value = clamp(value, 0.f, 1.f);
	end -= start;

	return end * (6.f * (1.f - value) / 5.f + 1.f) * (-2.2f * pow(1.f - value, 1.2f) *
		sin(PI * value * (2.5f * value * value * value + 0.2f)) + pow(1.f - value, 2.2f) *
		(PI * (2.5f * value * value * value + 0.2f) + 7.5f * PI * value * value * value) *
		cos(PI * value * (2.5f * value * value * value + 0.2f)) + 1.f) -
		6.f * end * (pow(1.f - value, 2.2f) * sin(PI * value * (2.5f * value * value * value + 0.2f)) + value / 5.f);
}

float Easing::easeInQuadD(float start, float end, float value) {
	return 2.f * (end - start) * value;
}

float Easing::easeOutQuadD(float start, float end, float value) {
	end -= start;
	return -end * value - end * (value - 2.f);
}

float Easing::easeInOutQuadD(float start, float end, float value) {
	value /= .5f;
	end -= start;

	if(value < 1.f) {
	return end * value;
	}

	value--;

	return end * (1.f - value);
}

float Easing::easeInCubicD(float start, float end, float value) {
	return 3.f * (end - start) * value * value;
}

float Easing::easeOutCubicD(float start, float end, float value) {
	value--;
	end -= start;
	return 3.f * end * value * value;
}

float Easing::easeInOutCubicD(float start, float end, float value) {
	value /= .5f;
	end -= start;

	if(value < 1.f) {
		return (3.f / 2.f) * end * value * value;
	}

	value -= 2.f;

	return (3.f / 2.f) * end * value * value;
}

float Easing::easeInQuartD(float start, float end, float value) {
	return 4.f * (end - start) * value * value * value;
}

float Easing::easeOutQuartD(float start, float end, float value) {
	value--;
	end -= start;
	return -4.f * end * value * value * value;
}

float Easing::easeInOutQuartD(float start, float end, float value) {
	value /= .5f;
	end -= start;

	if(value < 1.f) {
		return 2.f * end * value * value * value;
	}

	value -= 2.f;

	return -2.f * end * value * value * value;
}

float Easing::easeInQuintD(float start, float end, float value) {
	return 5.f * (end - start) * value * value * value * value;
}

float Easing::easeOutQuintD(float start, float end, float value) {
	value--;
	end -= start;
	return 5.f * end * value * value * value * value;
}

float Easing::easeInOutQuintD(float start, float end, float value) {
	value /= .5f;
	end -= start;

	if(value < 1.f) {
		return (5.f / 2.f) * end * value * value * value * value;
	}

	value -= 2.f;

	return (5.f / 2.f) * end * value * value * value * value;
}

float Easing::easeInSineD(float start, float end, float value) {
	return (end - start) * 0.5f * PI * sin(0.5f * PI * value);
}

float Easing::easeOutSineD(float start, float end, float value) {
	end -= start;
	return (PI * 0.5f) * end * cos(value * (PI * 0.5f));
}

float Easing::easeInOutSineD(float start, float end, float value) {
	end -= start;
	return end * 0.5f * PI * sin(PI * value);
}

float Easing::easeInExpoD(float start, float end, float value) {
	return (10.f * NATURAL_LOG_OF_2 * (end - start) * pow(2.f, 10.f * (value - 1.f)));
}

float Easing::easeOutExpoD(float start, float end, float value) {
	end -= start;
	return 5.f * NATURAL_LOG_OF_2 * end * pow(2.f, 1.f - 10.f * value);
}

float Easing::easeInOutExpoD(float start, float end, float value) {
	value /= .5f;
	end -= start;

	if(value < 1.f) {
		return 5.f * NATURAL_LOG_OF_2 * end * pow(2.f, 10.f * (value - 1.f));
	}

	value--;

	return (5.f * NATURAL_LOG_OF_2 * end) / (pow(2.f, 10.f * value));
}

float Easing::easeInCircD(float start, float end, float value) {
	return ((end - start) * value) / sqrt(1.f - value * value);
}

float Easing::easeOutCircD(float start, float end, float value) {
	value--;
	end -= start;
	return (-end * value) / sqrt(1.f - value * value);
}

float Easing::easeInOutCircD(float start, float end, float value) {
	value /= .5f;
	end -= start;

	if(value < 1.f) {
	return (end * value) / (2.f * sqrt(1.f - value * value));
	}

	value -= 2.f;

	return (-end * value) / (2.f * sqrt(1.f - value * value));
}

float Easing::easeInBounceD(float start, float end, float value) {
	end -= start;
	float d = 1.f;

	return easeOutBounceD(0.f, end, d - value);
}

float Easing::easeOutBounceD(float start, float end, float value) {
	value /= 1.f;
	end -= start;

	if(value < (1.f / 2.75f)) {
		return 2.f * end * 7.5625f * value;
	} else if(value < (2.f / 2.75f)) {
		value -= (1.5f / 2.75f);
		return 2.f * end * 7.5625f * value;
	} else if(value < (2.5 / 2.75)) {
		value -= (2.25f / 2.75f);
		return 2.f * end * 7.5625f * value;
	} else {
		value -= (2.625f / 2.75f);
		return 2.f * end * 7.5625f * value;
	}
}

float Easing::easeInOutBounceD(float start, float end, float value) {
	end -= start;
	float d = 1.f;

	if(value < d * 0.5f) {
		return easeInBounceD(0.f, end, value * 2.f) * 0.5f;
	} else {
		return easeOutBounceD(0.f, end, value * 2.f - d) * 0.5f;
	}
}

float Easing::easeInBackD(float start, float end, float value) {
	float s = 1.70158f;

	return 3.f * (s + 1.f) * (end - start) * value * value - 2.f * s * (end - start) * value;
}

float Easing::easeOutBackD(float start, float end, float value) {
	float s = 1.70158f;
	end -= start;
	value = (value) - 1.f;

	return end * ((s + 1.f) * value * value + 2.f * value * ((s + 1.f) * value + s));
}

float Easing::easeInOutBackD(float start, float end, float value) {
	float s = 1.70158f;
	end -= start;
	value /= .5f;

	if((value) < 1.f) {
		s *= (1.525f);
		return 0.5f * end * (s + 1.f) * value * value + end * value * ((s + 1.f) * value - s);
	}

	value -= 2.f;
	s *= (1.525f);
	return 0.5f * end * ((s + 1.f) * value * value + 2.f * value * ((s + 1.f) * value + s));
}

float Easing::easeInElasticD(float start, float end, float value) {
	return easeOutElasticD(start, end, 1.f - value);
}

float Easing::easeOutElasticD(float start, float end, float value) {
	end -= start;

	float d = 1.f;
	float p = d * .3f;
	float s;
	float a = 0.f;

	if(a == 0.f || a < abs(end)) {
		a = end;
		s = p * 0.25f;
	} else {
		s = p / (2 * PI) * asin(end / a);
	}

	return (a * PI * d * pow(2.f, 1.f - 10.f * value) *
	cos((2.f * PI * (d * value - s)) / p)) / p - 5.f * NATURAL_LOG_OF_2 * a *
	pow(2.f, 1.f - 10.f * value) * sin((2.f * PI * (d * value - s)) / p);
}

float Easing::easeInOutElasticD(float start, float end, float value) {
	end -= start;

	float d = 1.f;
	float p = d * .3f;
	float s;
	float a = 0.f;

	if(a == 0.f || a < abs(end)) {
		a = end;
		s = p / 4.f;
	} else {
		s = p / (2.f * PI) * asin(end / a);
	}

	if(value < 1.f) {
		value -= 1.f;

		return -5.f * NATURAL_LOG_OF_2 * a * pow(2.f, 10.f * value) * sin(2.f * PI * (d * value - 2.f) / p) -
		a * PI * d * pow(2.f, 10.f * value) * cos(2.f * PI * (d * value - s) / p) / p;
	}

	value -= 1.f;

	return a * PI * d * cos(2.f * PI * (d * value - s) / p) / (p * pow(2.f, 10.f * value)) -
	5.f * NATURAL_LOG_OF_2 * a * sin(2.f * PI * (d * value - s) / p) / (pow(2.f, 10.f * value));
}

