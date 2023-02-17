#pragma once

#include "akila/core/math/math.hpp"
#include <vector>

namespace akila {
	class Image {
	public:
		Image(IVec2 const &size);

		IVec2 const &getSize() const;

		void writePixel(IVec2 const &coords, IVec4 const &color);
		IVec4 const &readPixel(IVec2 const &coords) const;
		IVec4 const &readPixel(Vec2 const &uv) const;

		void setData(void const *data, std::size_t length = -1);
		void const * data() const;

		void setBorderColor(IVec4 const &color);

	private:
		IVec2 size;
		std::vector<IVec4> image;
		IVec4 borderColor;
	};
}
