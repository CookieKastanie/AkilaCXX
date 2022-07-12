#include "akila/default/loaders/texture_loader.hpp"
#include "akila/akila.hpp"
#include <stbimage/stb_image.h>

using namespace akila;

TextureBuffer::Format stringToFormat(std::string const &str) {
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


Texture2DLoader::Texture2DLoader(): Loader{"texture2d"} {
	Resources::registerType<Texture2D>();
}

void Texture2DLoader::onEntry(JSON json, LoaderCallback cb) {
	if(!json["name"].is_string()) {
		cb.fail();
		return;
	}

	std::string name = json["name"];

	Texture2D *texture;
	if(json["format"].is_string())
		texture = new Texture2D{stringToFormat(json["format"])};
	else
		texture = new Texture2D{};

	if(json["path"].is_string()) {

		std::string path = json["path"];

		std::cout << path << std::endl;
		std::cout << FileSystem::path(path) << std::endl;
		int textureNrChannels = 3;
		int fwidth = 1;
		int fheight = 1;
		void *data = nullptr;
		TextureBuffer::Format fileFormat = TextureBuffer::Format::RGB;
		TextureBuffer::Type dataType = TextureBuffer::Type::UNSIGNED_BYTE;


		// bruh
		//Threadpool::submit([&]() {
			std::cout << path << std::endl;

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
		//}, [&]() {
			if(data != nullptr) {
				texture->setSize(fwidth, fheight);
				texture->setData(data, fileFormat, dataType);

				stbi_image_free(data);

				Resources::set<Texture2D>(name, texture);
				cb.success();
			} else {
				std::cerr << "Texture loading error : can't read " << path << std::endl;
				Resources::set<Texture2D>(name, texture);
				cb.fail();
			}
		//});
	} else {
		Resources::set<Texture2D>(name, texture);
		cb.success();
	}
}
