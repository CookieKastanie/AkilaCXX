#pragma once

#include "akila/core/math/math.hpp"

struct GLFWcursor;
namespace akila {
	class Cursor {
	public:
		Cursor();
		~Cursor();

		Cursor(Cursor &&other) noexcept;
		Cursor &operator=(Cursor &&other) noexcept;

		Cursor(Cursor const &other) = delete;
		Cursor &operator=(Cursor const &other) = delete;

		/* RGBA format */
		void setData(IVec2 size, unsigned char *data, IVec2 center = {0, 0});
		void bind();

	private:
		GLFWcursor *cursor;
	};
}
