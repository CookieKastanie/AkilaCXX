#include "akila/engine/loaders/texture_loader.hpp"
#include "akila/engine/loaders/bitmap_parser.hpp"
#include "akila/core/rhi/texture.hpp"
#include "akila/core/resources/resources.hpp"
#include "akila/core/threadpool/threadpool.hpp"
#include "akila/core/resources/file_system.hpp"

using namespace akila;

TextureBuffer::Format stringToFormat(std::string const &str) {
	if(str == "DEPTH_COMPONENT") return TextureBuffer::Format::DEPTH_COMPONENT;
	if(str == "DEPTH_STENCIL") return TextureBuffer::Format::DEPTH_STENCIL;

	if(str == "RED") return TextureBuffer::Format::RED;
	if(str == "RG") return TextureBuffer::Format::RG;
	if(str == "RGBA") return TextureBuffer::Format::RGBA;
	if(str == "SRGB") return TextureBuffer::Format::SRGB;
	if(str == "SRGB_ALPHA") return TextureBuffer::Format::SRGB_ALPHA;
	if(str == "RGB16F") return TextureBuffer::Format::RGB16F;
	if(str == "RGBA16F") return TextureBuffer::Format::RGBA16F;

	if(str == "COMPRESSED_RED") return TextureBuffer::Format::COMPRESSED_RED;
	if(str == "COMPRESSED_RG") return TextureBuffer::Format::COMPRESSED_RG;
	if(str == "COMPRESSED_RGB") return TextureBuffer::Format::COMPRESSED_RGB;
	if(str == "COMPRESSED_RGBA") return TextureBuffer::Format::COMPRESSED_RGBA;
	if(str == "COMPRESSED_SRGB") return TextureBuffer::Format::COMPRESSED_SRGB;
	if(str == "COMPRESSED_SRGB_ALPHA") return TextureBuffer::Format::COMPRESSED_SRGB_ALPHA;

	//default
	return TextureBuffer::Format::RGB;
}

TextureBuffer::WrapMode stringToWrapMode(std::string const &str) {
	if(str == "CLAMP_TO_EDGE") return TextureBuffer::WrapMode::CLAMP_TO_EDGE;
	if(str == "CLAMP_TO_BORDER") return TextureBuffer::WrapMode::CLAMP_TO_BORDER;
	if(str == "MIRRORED_REPEAT") return TextureBuffer::WrapMode::MIRRORED_REPEAT;

	//default
	return TextureBuffer::WrapMode::REPEAT;
}

TextureBuffer::FilterMode stringToFilterMode(std::string const &str) {
	if(str == "NEAREST") return TextureBuffer::FilterMode::NEAREST;
	if(str == "NEAREST_MIPMAP_NEAREST") return TextureBuffer::FilterMode::NEAREST_MIPMAP_NEAREST;
	if(str == "LINEAR_MIPMAP_NEAREST") return TextureBuffer::FilterMode::LINEAR_MIPMAP_NEAREST;
	if(str == "NEAREST_MIPMAP_LINEAR") return TextureBuffer::FilterMode::NEAREST_MIPMAP_LINEAR;
	if(str == "LINEAR_MIPMAP_LINEAR") return TextureBuffer::FilterMode::LINEAR_MIPMAP_LINEAR;

	// default
	return TextureBuffer::FilterMode::LINEAR;
}

Texture2DLoader::Texture2DLoader(): Loader{"texture2d"} {}

void Texture2DLoader::onEntry(JSON json, LoaderCallback cb) {
	if(!json["name"].is_string()) {
		cb.fail();
		return;
	}

	Texture2D *texture;
	if(json["format"].is_string()) {
		texture = new Texture2D{stringToFormat(json["format"])};
	} else {
		texture = new Texture2D{};
	}

	TextureBuffer::Parameters params{};
	if(json["wrapS"].is_string()) params.wrapS = stringToWrapMode(json["wrapS"]);
	if(json["wrapT"].is_string()) params.wrapT = stringToWrapMode(json["wrapT"]);
	if(json["wrapR"].is_string()) params.wrapR = stringToWrapMode(json["wrapR"]);
	if(json["magFilter"].is_string()) params.magFilter = stringToFilterMode(json["magFilter"]);
	if(json["minFilter"].is_string()) params.minFilter = stringToFilterMode(json["minFilter"]);

	texture->setParameters(params);

	std::string name = json["name"];
	Resources::set<Texture2D>(name, texture);

	if(json["path"].is_string() == false) {
		cb.success();
		return;
	}

	Ptr<BitmapParser> parser = createPtr<BitmapParser>();
	std::string path = json["path"];
	bool mips = false;
	if(json["mips"].is_boolean()) mips = json["mips"];

	Threadpool::submit([=]() {
			parser->loadFile(FileSystem::resources(path));
		}, [=]() {
			Bitmap const &bitmap = parser->getResult();

			if(bitmap.data == nullptr) {
				cb.fail();
				return;
			}

			TextureBuffer::Format fileFormat = TextureBuffer::Format::RGB;
			switch(bitmap.channelCount) {
				case 1: fileFormat = TextureBuffer::Format::RED; break;
				case 2: fileFormat = TextureBuffer::Format::RG; break;
				case 3: fileFormat = TextureBuffer::Format::RGB; break;
				case 4: fileFormat = TextureBuffer::Format::RGBA; break;
			}

			TextureBuffer::Type dataType = TextureBuffer::Type::UNSIGNED_BYTE;
			if(bitmap.isFloat) dataType = TextureBuffer::Type::FLOAT;

			texture->setSize({bitmap.width, bitmap.height});
			texture->setData(bitmap.data, fileFormat, dataType);

			if(mips) {
				texture->generateMipmap();
			}

			cb.success();
		}
	);
}
