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


Texture2DLoader::Texture2DLoader(): Loader{"texture2d"} {}

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
		struct _ {
			std::string path;
			int textureNrChannels = 3;
			int fwidth = 1;
			int fheight = 1;
			void *data = nullptr;
			TextureBuffer::Format fileFormat = TextureBuffer::Format::RGB;
			TextureBuffer::Type dataType = TextureBuffer::Type::UNSIGNED_BYTE;
		};
		
		Ptr<_> p = createPtr<_>();
		p->path = json["path"];

		Threadpool::submit([=]() {
			stbi_set_flip_vertically_on_load(true);
			if(texture->getInternalFormat() == TextureBuffer::Format::RGB16F || texture->getInternalFormat() == TextureBuffer::Format::RGBA16F) {// <- pas fou
				p->data = stbi_loadf(FileSystem::path(p->path).c_str(), &p->fwidth, &p->fheight, &p->textureNrChannels, 0);
				p->dataType = TextureBuffer::Type::FLOAT;
			} else {
				p->data = stbi_load(FileSystem::path(p->path).c_str(), &p->fwidth, &p->fheight, &p->textureNrChannels, 0);
			}

			switch(p->textureNrChannels) {
				case 1: p->fileFormat = TextureBuffer::Format::RED; break;
				case 3: p->fileFormat = TextureBuffer::Format::RGB; break;
				case 4: p->fileFormat = TextureBuffer::Format::RGBA; break;
			}
		}, [=]() {
			if(p->data != nullptr) {
				texture->setSize(p->fwidth, p->fheight);
				texture->setData(p->data, p->fileFormat, p->dataType);

				stbi_image_free(p->data);

				Resources::set<Texture2D>(name, texture);
				cb.success();
			} else {
				std::cerr << "Texture loading error : can't read " << p->path << std::endl;
				Resources::set<Texture2D>(name, texture);
				cb.fail();
			}
		});
	} else {
		Resources::set<Texture2D>(name, texture);
		cb.success();
	}
}
