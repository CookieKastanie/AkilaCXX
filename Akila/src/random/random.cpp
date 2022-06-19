#include "akila/random/random.hpp"

using namespace akila;

std::mt19937 Random::engine;

void Random::init() {
	engine = std::mt19937{std::random_device{}()};
}

float Random::getFloat(float from, float to) {
	std::uniform_real_distribution<float> dist{from, to};
	return dist(engine);
}

int Random::getInt(int from, int to) {
	std::uniform_int_distribution<int> dist{from, to};
	return dist(engine);
}
