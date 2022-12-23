#include "akila/core/window/cursor.hpp"
#include "akila/core/window/window.hpp"

using namespace akila;

Cursor::Cursor(): cursor{nullptr} {

}

Cursor::~Cursor() {
	if(cursor != nullptr) {
		glfwDestroyCursor(cursor);
	}
}

Cursor::Cursor(Cursor &&other) noexcept: cursor{other.cursor} {
	other.cursor = nullptr;
}

Cursor &Cursor::operator=(Cursor &&other) noexcept {
	cursor = other.cursor;
	other.cursor = nullptr;

	return *this;
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
