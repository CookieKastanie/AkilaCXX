#pragma once

#include <random>

namespace akila {
	class Random {
	public:
		static float getFloat(float from = 0.f, float to = 1.f);
		static int getInt(int from = 0, int to = 100);

	private:
		friend class Core;

		static std::mt19937 engine;
		static void init();
	};
}
