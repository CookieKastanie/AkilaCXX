#include "Akila/core/Core.hpp"
#include "Akila/files/Loader.hpp"
#include "Akila/files/FileSystem.hpp"
#include "stbimage/stb_image.h"
#include "Akila/graphics/ShaderBuilder.hpp"

using namespace Akila;

class TextureTask: public Task {
private:
	TextureBuffer::Format fileFormat;
	TextureBuffer::Type dataType;
	int textureNrChannels;

	int fwidth = 1;
	int fheight = 1;

	void *data;

	Texture *texture;
	const std::string path;

	bool generateMips;

public:
	TextureTask(Texture *texture, const std::string &path, const bool generateMips):
		fileFormat{TextureBuffer::RGBA},
		dataType{TextureBuffer::UNSIGNED_BYTE},
		textureNrChannels{3},
		fwidth{1},
		fheight{1},
		data{nullptr},
		texture{texture},
		path{path},
		generateMips{generateMips} {
	}

	void onBackground() override {
		stbi_set_flip_vertically_on_load(true);
		if(texture->getInternalFormat() == TextureBuffer::RGB16F || texture->getInternalFormat() == TextureBuffer::RGBA16F) {// <- pas fou
			data = stbi_loadf(FileSystem::path(path).c_str(), &fwidth, &fheight, &textureNrChannels, 0);
			dataType = TextureBuffer::FLOAT;
		} else {
			data = stbi_load(FileSystem::path(path).c_str(), &fwidth, &fheight, &textureNrChannels, 0);
		}

		switch(textureNrChannels) {
			case 1: fileFormat = TextureBuffer::RED; break;
			case 3: fileFormat = TextureBuffer::RGB; break;
			case 4: fileFormat = TextureBuffer::RGBA; break;
		}
	}

	void onMain() override {
		if(data != nullptr) {
			texture->setSize(fwidth, fheight);
			texture->setData(data, fileFormat, dataType);
			if(generateMips) texture->generateMipmap();

			stbi_image_free(data);
		} else {
			std::cerr << "Texture loading error : can't read " << path << std::endl;
		}
	}
};

void Loader::asyncTexture(Texture *texture, const std::string &path, const bool generateMips, TaskManager *tm) {
	if(tm == nullptr) tm = Core::taskManager.get();
	tm->submit(new TextureTask(texture, path, generateMips));
}


std::shared_ptr<Shader> Loader::shader(const std::string &path) {
	std::ifstream file;
	file.open(FileSystem::path(path).c_str());
	if(!file.good()) std::cerr << "Shader loading error : can't read " << path << std::endl;

	std::stringstream stream;
	stream << file.rdbuf();
	file.close();

	return ShaderBuilder::build(stream.str());
}
















void Loader::splitString(std::vector<std::string> &list, std::string &str, const std::string &delimiter) {
	list.clear();

	std::size_t pos = 0;
	while((pos = str.find(delimiter)) != std::string::npos) {
		list.push_back(str.substr(0, pos));
		str.erase(0, pos + delimiter.length());
	}

	list.push_back(str);
}

void Loader::trimString(std::string &str) {
	std::size_t i = 0;

	while((i < str.size()) && std::isspace(str[i])) ++i;
	str.erase(0, i);

	if(str.size() == 0) return;

	i = str.size() - 1;

	while((i >= 0) && std::isspace(str[i])) --i;
	str.erase(i + 1, str.size());
}
