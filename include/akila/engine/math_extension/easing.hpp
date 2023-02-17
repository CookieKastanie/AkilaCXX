#pragma once

namespace akila {
	class Easing {
	public:
		static float linear(float start, float end, float value);
		static float spring(float start, float end, float value);
		static float easeInQuad(float start, float end, float value);
		static float easeOutQuad(float start, float end, float value);
		static float easeInOutQuad(float start, float end, float value);
		static float easeInCubic(float start, float end, float value);
		static float easeOutCubic(float start, float end, float value);
		static float easeInOutCubic(float start, float end, float value);
		static float easeInQuart(float start, float end, float value);
		static float easeOutQuart(float start, float end, float value);
		static float easeInOutQuart(float start, float end, float value);
		static float easeInQuint(float start, float end, float value);
		static float easeOutQuint(float start, float end, float value);
		static float easeInOutQuint(float start, float end, float value);
		static float easeInSine(float start, float end, float value);
		static float easeOutSine(float start, float end, float value);
		static float easeInOutSine(float start, float end, float value);
		static float easeInExpo(float start, float end, float value);
		static float easeOutExpo(float start, float end, float value);
		static float easeInOutExpo(float start, float end, float value);
		static float easeInCirc(float start, float end, float value);
		static float easeOutCirc(float start, float end, float value);
		static float easeInOutCirc(float start, float end, float value);
		static float easeInBounce(float start, float end, float value);
		static float easeOutBounce(float start, float end, float value);
		static float easeInOutBounce(float start, float end, float value);
		static float easeInBack(float start, float end, float value);
		static float easeOutBack(float start, float end, float value);
		static float easeInOutBack(float start, float end, float value);
		static float easeInElastic(float start, float end, float value);
		static float easeOutElastic(float start, float end, float value);
		static float easeInOutElastic(float start, float end, float value);

		static float linearD(float start, float end, float value);
		static float springD(float start, float end, float value);
		static float easeInQuadD(float start, float end, float value);
		static float easeOutQuadD(float start, float end, float value);
		static float easeInOutQuadD(float start, float end, float value);
		static float easeInCubicD(float start, float end, float value);
		static float easeOutCubicD(float start, float end, float value);
		static float easeInOutCubicD(float start, float end, float value);
		static float easeInQuartD(float start, float end, float value);
		static float easeOutQuartD(float start, float end, float value);
		static float easeInOutQuartD(float start, float end, float value);
		static float easeInQuintD(float start, float end, float value);
		static float easeOutQuintD(float start, float end, float value);
		static float easeInOutQuintD(float start, float end, float value);
		static float easeInSineD(float start, float end, float value);
		static float easeOutSineD(float start, float end, float value);
		static float easeInOutSineD(float start, float end, float value);
		static float easeInExpoD(float start, float end, float value);
		static float easeOutExpoD(float start, float end, float value);
		static float easeInOutExpoD(float start, float end, float value);
		static float easeInCircD(float start, float end, float value);
		static float easeOutCircD(float start, float end, float value);
		static float easeInOutCircD(float start, float end, float value);
		static float easeInBounceD(float start, float end, float value);
		static float easeOutBounceD(float start, float end, float value);
		static float easeInOutBounceD(float start, float end, float value);
		static float easeInBackD(float start, float end, float value);
		static float easeOutBackD(float start, float end, float value);
		static float easeInOutBackD(float start, float end, float value);
		static float easeInElasticD(float start, float end, float value);
		static float easeOutElasticD(float start, float end, float value);
		static float easeInOutElasticD(float start, float end, float value);
	};
}
