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

TextureBuffer::WrapMode stringToWrapMode(std::string const &str) {
	if(!str.compare("CLAMP_TO_EDGE")) return TextureBuffer::WrapMode::CLAMP_TO_EDGE;
	if(!str.compare("CLAMP_TO_BORDER")) return TextureBuffer::WrapMode::CLAMP_TO_BORDER;
	if(!str.compare("MIRRORED_REPEAT")) return TextureBuffer::WrapMode::MIRRORED_REPEAT;

	//default
	return TextureBuffer::WrapMode::REPEAT;
}

TextureBuffer::FilterMode stringToFilterMode(std::string const &str) {
	if(!str.compare("NEAREST")) return TextureBuffer::FilterMode::NEAREST;
	if(!str.compare("NEAREST_MIPMAP_NEAREST")) return TextureBuffer::FilterMode::NEAREST_MIPMAP_NEAREST;
	if(!str.compare("LINEAR_MIPMAP_NEAREST")) return TextureBuffer::FilterMode::LINEAR_MIPMAP_NEAREST;
	if(!str.compare("NEAREST_MIPMAP_LINEAR")) return TextureBuffer::FilterMode::NEAREST_MIPMAP_LINEAR;
	if(!str.compare("LINEAR_MIPMAP_LINEAR")) return TextureBuffer::FilterMode::LINEAR_MIPMAP_LINEAR;

	// default
	return TextureBuffer::FilterMode::LINEAR;
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

	TextureBuffer::Parameters params{};
	if(json["wrapS"].is_string())
		params.wrapS = stringToWrapMode(json["wrapS"]);

	if(json["wrapT"].is_string())
		params.wrapT = stringToWrapMode(json["wrapT"]);

	if(json["wrapR"].is_string())
		params.wrapR = stringToWrapMode(json["wrapR"]);

	if(json["magFilter"].is_string())
		params.magFilter = stringToFilterMode(json["magFilter"]);

	if(json["minFilter"].is_string())
		params.minFilter = stringToFilterMode(json["minFilter"]);

	texture->setParameters(params);

	bool mips = false;
	if(json["mips"].is_boolean()) mips = json["mips"];

	bool invertY = false;
	if(json["invertY"].is_boolean()) invertY = json["invertY"];

	Resources::set<Texture2D>(name, texture);

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
			stbi_set_flip_vertically_on_load(invertY);
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
				texture->setSize({p->fwidth, p->fheight});
				texture->setData(p->data, p->fileFormat, p->dataType);

				stbi_image_free(p->data);

				if(mips) texture->generateMipmap();

				cb.success();
			} else {
				std::cerr << "Texture loading error : can't read " << p->path << std::endl;
				cb.fail();
			}
		});
	} else {
		cb.success();
	}
}
