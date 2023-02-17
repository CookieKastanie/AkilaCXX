#include "akila/engine/containers/image.hpp"

using namespace akila;

Image::Image(IVec2 const &size): size{size}, borderColor{255, 255, 255, 255} {
	image.resize(size.x * size.y);
}

IVec2 const &Image::getSize() const {
	return size;
}

void Image::writePixel(IVec2 const &coords, IVec4 const &color) {
	if(coords.x < 0 || coords.x >= size.x) return;
	if(coords.y < 0 || coords.y >= size.y) return;

	int const index = (size.y - 1 - coords.y) * size.x + coords.x;
	image[index] = color;
}

IVec4 const &Image::readPixel(IVec2 const &coords) const {
	if(coords.x < 0 || coords.x >= size.x) return borderColor;
	if(coords.y < 0 || coords.y >= size.y) return borderColor;

	int const index = (size.y - 1 - coords.y) * size.x + coords.x;
	return image[index];
}

IVec4 const &Image::readPixel(Vec2 const &uv) const {
	return readPixel(IVec2{
		uv.x * (size.x - 1),
		uv.y * (size.y - 1)
	});
}

void Image::setData(void const *newData, std::size_t length) {
	if(length == -1) {
		length = size.x * size.y * 4;
	}

	std::memcpy(image.data(), newData, length);
}

void const *Image::data() const {
	return image.data();
}

void Image::setBorderColor(IVec4 const &color) {
	borderColor = color;
}
