#include "akila/window/cursor.hpp"
#include "akila/window/window.hpp"

using namespace akila;

Cursor::Cursor(): cursor{nullptr} {

}

Cursor::~Cursor() {
	if(cursor != nullptr) {
		glfwDestroyCursor(cursor);
	}
}

void Cursor::setData(IVec2 size, unsigned char *data, IVec2 center) {
	if(cursor != nullptr) {
		glfwDestroyCursor(cursor);
	}

	GLFWimage image{size.x, size.y, data};
	cursor = glfwCreateCursor(&image, center.x, center.y);
}

void Cursor::bind() {
	glfwSetCursor(Window::window, cursor);
}
