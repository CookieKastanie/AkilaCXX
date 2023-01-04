#pragma once

#include <string>

namespace akila {
	struct Bitmap {
		int channelCount = 0;
		int width = 0;
		int height = 0;
		bool isFloat = false;
		std::size_t byteCount = 0;
		void *data = nullptr;
	};

	class BitmapParser {
	public:
		BitmapParser();
		~BitmapParser();

		BitmapParser(BitmapParser &&other) noexcept = delete;
		BitmapParser &operator=(BitmapParser &&other) noexcept = delete;

		BitmapParser(BitmapParser const &other) = delete;
		BitmapParser &operator=(BitmapParser const &other) = delete;

		bool loadFile(std::string const &path, int desiredChannelCount = 0);
		Bitmap const &getResult();

	private:
		Bitmap result;
		void *raw;
	};
}
