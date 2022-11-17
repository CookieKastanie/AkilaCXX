#pragma once

#include <random>
#include "akila/math/math.hpp"

namespace akila {
	class Random {
	public:
		static float getFloat(float from = 0.f, float to = 1.f);
		static int getInt(int from = 0, int to = 100);

		/* C'est juste 2 valeurs random */
		static Vec2 getVec2(float from = 0.f, float to = 1.f);
		/* C'est juste 3 valeurs random */
		static Vec3 getVec3(float from = 0.f, float to = 1.f);
		static float getAngle();

		static std::uint64_t getUid();

	private:
		friend class Core;

		static std::mt19937 engine;
		static void init();
	};
}
