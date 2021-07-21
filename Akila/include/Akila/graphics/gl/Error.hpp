#pragma once

#include "glad/glad.h"
#include <iostream>

void dumpGlErrorStack();
void dumpLastGlError();

#define GL_ERROR_STACK() {std::cout << __FILE__ << " (" << __LINE__ << ") :" << std::endl; dumpGlErrorStack(); std::cout << std::endl << std::endl;}
#define GL_ERROR() {std::cout << __FILE__ << " (" << __LINE__ << ") : ";  dumpLastGlError(); std::cout << std::endl;}
