#include "Akila/core/Time.hpp"
#include "GLFW/glfw3.h"

using namespace Akila;

float Time::last = 0;
float Time::now = 0;
float Time::delta = 0;

void Time::update() {
	last = now;
	now = (float)glfwGetTime();
	delta = now - last;
}
