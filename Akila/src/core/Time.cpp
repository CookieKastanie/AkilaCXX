#include "Akila/core/Time.hpp"
#include "GLFW/glfw3.h"

using namespace Akila;

float Time::last = 0;
float Time::now = 0;
float Time::delta = 0;
float Time::maxDelta = 1.f / 20.f;

float Time::fixedDelta = 1.f / 45.f;
float Time::mix = 0;

void Time::update() {
	last = now;
	now = (float)glfwGetTime();
	delta = now - last;
	if(delta > maxDelta) delta = maxDelta;
}
