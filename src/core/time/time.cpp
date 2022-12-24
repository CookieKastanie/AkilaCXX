#include "akila/core/time/Time.hpp"

#pragma warning(push, 0)
#include <GLFW/glfw3.h>
#pragma warning(pop)

using namespace akila;

float Time::now = 0.f;
float Time::delta = 0.f;
float Time::fixedDelta = 1.f / 64.f;
float Time::mix = 0.f;

double Time::dPrev = 0.;
double Time::dNow = 0.;

void Time::update() {
	dPrev = dNow;
	dNow = glfwGetTime();
	now = static_cast<float>(dNow);
	delta = static_cast<float>(dNow - dPrev);
}
