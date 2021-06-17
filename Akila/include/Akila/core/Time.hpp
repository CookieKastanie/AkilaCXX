#pragma once

namespace Akila {
	class Time {
		public:
			static float last;
			static float now;
			static float delta;

			static void update();
	};
}