#pragma once

#include "akila/math/math.hpp"

struct GLFWcursor;
namespace akila {
	class Cursor {
	public:
		Cursor();
		~Cursor();

		/* RGBA format */
		void setData(IVec2 size, unsigned char *data, IVec2 center = {0, 0});
		void bind();

	private:
		GLFWcursor *cursor;
	};
}
