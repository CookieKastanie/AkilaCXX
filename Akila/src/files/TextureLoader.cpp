#include "Akila/files/TextureLoader.hpp"

#include "Akila/core/Core.hpp"
#include "stbimage/stb_image.h"

using namespace Akila;

void TextureLoader::color(Texture *texture, std::string const &path, std::function<void()> const &callback) {

	struct _ {
		Texture *texture;
		std::string const path;
		std::function<void()> const callback;

		int textureNrChannels;
		int fwidth;
		int fheight;
		void *data;
		TextureBuffer::Format fileFormat;
		TextureBuffer::Type dataType;
	};
	auto coro = Core::coroutines->create(_{texture, path, callback, 3, 1, 1, nullptr, {}, {}});

	coro->pushInThread(Trigger::AT_FRAME_START, [](_ &state) {
		Texture *texture = state.texture;
		std::string const &path = state.path;
		int &textureNrChannels = state.textureNrChannels;
		int &fwidth = state.fwidth;
		int &fheight = state.fheight;
		void *&data = state.data;
		auto &dataType = state.dataType;
		auto &fileFormat = state.fileFormat;

		stbi_set_flip_vertically_on_load(true);
		if(texture->getInternalFormat() == TextureBuffer::Format::RGB16F || texture->getInternalFormat() == TextureBuffer::Format::RGBA16F) {// <- pas fou
			data = stbi_loadf(FileSystem::path(path).c_str(), &fwidth, &fheight, &textureNrChannels, 0);
			dataType = TextureBuffer::Type::FLOAT;
		} else {
			data = stbi_load(FileSystem::path(path).c_str(), &fwidth, &fheight, &textureNrChannels, 0);
		}

		switch(textureNrChannels) {
			case 1: fileFormat = TextureBuffer::Format::RED; break;
			case 3: fileFormat = TextureBuffer::Format::RGB; break;
			case 4: fileFormat = TextureBuffer::Format::RGBA; break;
		}

		return 1;
	});

	coro->push(Trigger::AT_FRAME_START, [](_ &state) {
		if(state.data != nullptr) {
			state.texture->setSize(state.fwidth, state.fheight);
			state.texture->setData(state.data, state.fileFormat, state.dataType);

			stbi_image_free(state.data);
		} else {
			std::cerr << "Texture loading error : can't read " << state.path << std::endl;
		}

		state.callback();

		return 1;
	});

	Core::coroutines->start(coro);
}
