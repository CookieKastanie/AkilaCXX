#include "akila/engine/math_extension/distribution.hpp"
#include <chrono>
#include <glm/gtc/random.hpp>

using namespace akila;

std::mt19937 Distribution::engine = std::mt19937{std::random_device{}()};

float Distribution::randomFloat(float from, float to) {
	std::uniform_real_distribution<float> dist{from, to};
	return dist(engine);
}

int Distribution::randomInt(int from, int to) {
	std::uniform_int_distribution<int> dist{from, to};
	return dist(engine);
}

Vec2 Distribution::randomVec2(float from, float to) {
	return {randomFloat(from, to), randomFloat(from, to)};
}

Vec2 Distribution::randomCircle(float radius) {
	return glm::circularRand(radius);
}

Vec2 Distribution::randomDisk(float radius) {
	return glm::diskRand(radius);
}

Vec3 Distribution::randomVec3(float from, float to) {
	return {randomFloat(from, to), randomFloat(from, to), randomFloat(from, to)};
}

Vec3 Distribution::randomSphere(float radius) {
	return glm::sphericalRand(radius);
}

Vec3 Distribution::randomBall(float radius) {
	return glm::ballRand(radius);
}

float Distribution::randomAngle() {
	return randomFloat(0, PI * 2.f);
}

float Distribution::randomSignedAngle() {
	return randomFloat(PI, -PI);
}

std::uint64_t Distribution::randomUint64() {
	using namespace std::chrono;
	time_point now = time_point_cast<seconds>(system_clock::now());

	seconds value = now.time_since_epoch();
	std::uint64_t uid = value.count() << 32;

	std::uniform_int_distribution<std::uint32_t> dist{0, static_cast<std::uint32_t>(-1)};
	uid |= static_cast<std::uint64_t>(dist(engine));

	return uid;
}

///

Vec3 Distribution::evenDirectionSphereIteration(std::size_t index, std::size_t count) {
	constexpr float const angleIncrement = PI * 2.f * GOLDEN_RATIO;

	float const t = static_cast<float>(index) / static_cast<float>(count);
	float const inclination = acos(1.f - 2.f * t);
	float const azimuth = angleIncrement * static_cast<float>(index);

	float const incSin = sin(inclination);
	return {
		incSin * cos(azimuth),
		incSin * sin(azimuth),
		cos(inclination)
	};
}

std::vector<Vec3> Distribution::evenDirectionSphere(std::size_t count) {
	std::vector<Vec3> directions;
	directions.resize(count);

	for(std::size_t i = 0; i < count; ++i) {
		directions[i] = evenDirectionSphereIteration(i, count);
	}

	return directions;
}
