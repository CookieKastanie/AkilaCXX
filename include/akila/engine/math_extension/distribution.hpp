#pragma once

#include <random>
#include "akila/core/math/math.hpp"

namespace akila {
	class Distribution {
	public:
		static float randomFloat(float from = 0.f, float to = 1.f);
		static int randomInt(int from = 0, int to = 100);

		static float randomAngle();
		static float randomSignedAngle();

		static Vec2 randomVec2(float from = 0.f, float to = 1.f);
		static Vec2 randomCircle(float radius = 1.f);
		static Vec2 randomDisk(float radius = 1.f);

		static Vec3 randomVec3(float from = 0.f, float to = 1.f);
		static Vec3 randomSphere(float radius = 1.f);
		static Vec3 randomBall(float radius = 1.f);

		//static randomString() TODO

		static std::uint64_t randomUint64();

		///

		Vec3 evenDirectionSphereIteration(std::size_t index, std::size_t count);
		std::vector<Vec3> evenDirectionSphere(std::size_t count);

	private:
		static std::mt19937 engine;
	};
}
