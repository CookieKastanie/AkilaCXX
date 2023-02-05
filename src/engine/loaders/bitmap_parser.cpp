#include "akila/engine/loaders/bitmap_parser.hpp"
#include <stbimage/stb_image.h>
#include <stbimage/stb_image_write.h>
#include <iostream>

using namespace akila;

BitmapParser::BitmapParser(): raw{nullptr} {

}

BitmapParser::~BitmapParser() {
	if(raw != nullptr) {
		stbi_image_free(raw);
	}
}

bool BitmapParser::loadFile(std::string const &path, int desiredChannelCount) {
	stbi_set_flip_vertically_on_load(true);

	if(raw != nullptr) {
		stbi_image_free(raw);
	}

	bool isFloat = stbi_is_16_bit(path.c_str()) != 0;

	if(isFloat) {
		raw = stbi_load_16(path.c_str(), &result.width, &result.height, &result.channelCount, desiredChannelCount);
	} else {
		raw = stbi_load(path.c_str(), &result.width, &result.height, &result.channelCount, desiredChannelCount);
	}
	
	result.data = raw;

	if(raw == nullptr) {
		std::cerr << "File loading error : can't read " << path << std::endl;
		return false;
	}
	
	std::size_t cmpSize = isFloat ? sizeof(unsigned char) : sizeof(float);
	result.byteCount = cmpSize * result.width * result.height * result.channelCount;
	result.isFloat = isFloat;

	return true;
}

Bitmap const &BitmapParser::getResult() {
	return result;
}



//void writeFile(std::string const &path, akila::TextureBuffer *texture, bool invertY) {
//	auto buffer = texture->getData();
//	IVec2 size = texture->getSize();

//	stbi_flip_vertically_on_write(invertY);
//	stbi_write_png(FileSystem::path(path).c_str(), size.x, size.y, 4, buffer.get(), 0);

	/*/
	int const w = 100;
	int const h = 100;
	unsigned char data[w * h * 3];

	int index = 0;
	for(int j = h - 1; j >= 0; --j) {
		for(int i = 0; i < w; ++i) {
			data[index++] = (unsigned char)(255.0 * i / w);
			data[index++] = (unsigned char)(255.0 * j / h);
			data[index++] = (unsigned char)(255.0 * 0.2);
		}
	}

	stbi_write_png(FileSystem::path(path).c_str(), w, h, 3, data, w * 3);
	//*/
//}
