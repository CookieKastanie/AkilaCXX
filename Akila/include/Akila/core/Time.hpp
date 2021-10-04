#pragma once

namespace Akila {
	class Time {
	public:
		static float last;
		static float now;
		static float delta;
		static float maxDelta;

		static float fixedDelta;
		static float mix;

	private:
		friend class Core;

		static void update();
	};
}