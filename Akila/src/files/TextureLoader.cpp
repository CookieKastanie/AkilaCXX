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

TextureBuffer::Format TextureLoader::stringToFormat(std::string const &str) {
	if(!str.compare("DEPTH_COMPONENT")) return TextureBuffer::Format::DEPTH_COMPONENT;
	if(!str.compare("DEPTH_STENCIL")) return TextureBuffer::Format::DEPTH_STENCIL;
	if(!str.compare("RED")) return TextureBuffer::Format::RED;
	if(!str.compare("RG")) return TextureBuffer::Format::RG;
	if(!str.compare("RGBA")) return TextureBuffer::Format::RGBA;
	if(!str.compare("SRGB")) return TextureBuffer::Format::SRGB;
	if(!str.compare("SRGB_ALPHA")) return TextureBuffer::Format::SRGB_ALPHA;
	if(!str.compare("RGB16F")) return TextureBuffer::Format::RGB16F;
	if(!str.compare("RGBA16F")) return TextureBuffer::Format::RGBA16F;

	//default
	return TextureBuffer::Format::RGB;
}

TextureBuffer::WrapMode TextureLoader::stringToWrapMode(std::string const &str) {
	if(!str.compare("CLAMP_TO_EDGE")) return TextureBuffer::WrapMode::CLAMP_TO_EDGE;
	if(!str.compare("CLAMP_TO_BORDER")) return TextureBuffer::WrapMode::CLAMP_TO_BORDER;
	if(!str.compare("MIRRORED_REPEAT")) return TextureBuffer::WrapMode::MIRRORED_REPEAT;

	//default
	return TextureBuffer::WrapMode::REPEAT;
}

TextureBuffer::FilterMode TextureLoader::stringToFilterMode(std::string const &str) {
	if(!str.compare("NEAREST")) return TextureBuffer::FilterMode::NEAREST;
	if(!str.compare("NEAREST_MIPMAP_NEAREST")) return TextureBuffer::FilterMode::NEAREST_MIPMAP_NEAREST;
	if(!str.compare("LINEAR_MIPMAP_NEAREST")) return TextureBuffer::FilterMode::LINEAR_MIPMAP_NEAREST;
	if(!str.compare("NEAREST_MIPMAP_LINEAR")) return TextureBuffer::FilterMode::NEAREST_MIPMAP_LINEAR;
	if(!str.compare("LINEAR_MIPMAP_LINEAR")) return TextureBuffer::FilterMode::LINEAR_MIPMAP_LINEAR;

	// default
	return TextureBuffer::FilterMode::LINEAR;
}
