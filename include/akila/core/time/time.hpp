#pragma once

namespace akila {
	class Time {
	public:
		static float now;
		static float delta;
		static float fixedDelta;
		static float mix;

	private:
		friend class Core;

		static double dPrev;
		static double dNow;

		static void update();
	};
}
