#include "Akila/core/ResourcesBucket.hpp"
#include "Akila/files/FileSystem.hpp"
#include "Akila/files/Loader.hpp"
#include "Akila/graphics/gl/Texture.hpp"

using namespace Akila;

ResourcesBucket::ResourcesBucket(const std::shared_ptr<Renderer> &renderer): renderer{renderer} {
	defaultShader = std::make_shared<Shader>("void main(){gl_Position=vec4(0.);}", "void main(){gl_FragColor=vec4(1.);}");

	defaultTexture = std::make_shared<Texture>();

	defaultMaterial = std::make_shared<Material>();
	defaultMaterial->setShader(defaultShader);
}


struct ShaderState {
	static const int STATE_ID = 0;

	std::string name;
	std::string src;

	void clear() {
		name = "none";
		src = "";
	}
};

struct TextureState {
	static const int STATE_ID = 1;

	std::string name;
	std::string src;
	TextureBuffer::Format format;

	void setFormat(const std::string &str) {
		if(!str.compare("DEPTH_COMPONENT")) format = TextureBuffer::Format::DEPTH_COMPONENT;
		if(!str.compare("DEPTH_STENCIL")) format = TextureBuffer::Format::DEPTH_STENCIL;
		if(!str.compare("RED")) format = TextureBuffer::Format::RED;
		if(!str.compare("RG")) format = TextureBuffer::Format::RG;
		if(!str.compare("RGB")) format = TextureBuffer::Format::RGB;
		if(!str.compare("RGBA")) format = TextureBuffer::Format::RGBA;
		if(!str.compare("SRGB")) format = TextureBuffer::Format::SRGB;
		if(!str.compare("SRGB_ALPHA")) format = TextureBuffer::Format::SRGB_ALPHA;
		if(!str.compare("RGB16F")) format = TextureBuffer::Format::RGB16F;
		if(!str.compare("RGBA16F")) format = TextureBuffer::Format::RGBA16F;
	}

	void clear() {
		name = "none";
		src = "";
		format = TextureBuffer::Format::RGB;
	}
};


struct MaterialState {
	static const int STATE_ID = 2;

	std::string name;
	std::string shader;

	void clear() {
		name = "none";
		shader = "";
	}
};

std::shared_ptr<Shader> &ResourcesBucket::getShader(const std::string &name) {
	auto &&val = shaders[name];
	if(val != nullptr) return val;
	else return defaultShader;
}

std::shared_ptr<Texture> &ResourcesBucket::getTexture(const std::string &name) {
	auto &&val = textures[name];
	if(val != nullptr) return val;
	else return defaultTexture;
}

std::shared_ptr<Material> &ResourcesBucket::getMaterial(const std::string &name) {
	auto &&val = materials[name];
	if(val != nullptr) return val;
	else return defaultMaterial;
}

void ResourcesBucket::loadResourceFile(const std::string &path, TaskManager *taskManger) {
	std::ifstream file;
	file.open(FileSystem::path(path).c_str(), std::ifstream::in);
	if(!file.good()) {
		std::cerr << "Resource list loading error : can't read " << path << std::endl;
		return;
	}

	int state = -1;
	ShaderState shaderState;
	MaterialState materialState;
	TextureState textureState;

	std::string line;
	while(std::getline(file, line)) {
		std::vector<std::string> values;
		Loader::splitString(values, line, " ");
		for(auto &s : values) Loader::trimString(s);

		if(state == -1) {
			if(values.size() >= 2) {
				if(!values[1].compare("{")) {
					if(!values[0].compare("shader")) { state = ShaderState::STATE_ID; shaderState.clear(); }
					else if(!values[0].compare("material")) { state = MaterialState::STATE_ID; materialState.clear(); }
					else if(!values[0].compare("texture")) { state = TextureState::STATE_ID; textureState.clear(); }
				}
			}

			continue;
		}

		
		if(values.size() > 0) {
			if(!values[0].compare("}")) {

				if(state == ShaderState::STATE_ID) {
					shaders.emplace(shaderState.name, Loader::shader(shaderState.src));
				}
				
				else if(state == MaterialState::STATE_ID) {
					auto m = std::make_shared<Material>();
					m->setShader(shaders[materialState.shader]);
					materials.emplace(materialState.name, m);

					if(m->getShader() == nullptr) {
						std::cerr << materialState.name << " material don't have shader program !" << std::endl;
						m->setShader(defaultShader);
					} else {
						renderer->affectUBOToShader(m->getShader());
					}
				}

				else if(state == TextureState::STATE_ID) {
					auto t = std::make_shared<Texture>(textureState.format);
					Loader::asyncTexture(t.get(), textureState.src, taskManger);
					textures.emplace(textureState.name, t);
				}

				state = -1;
				continue;
			}


			////////////////////////////////////


			line = "";
			for(auto v : values) line += v;
			
			Loader::splitString(values, line, ":");

			if(values.size() >= 2) {
				if(state == ShaderState::STATE_ID) {
					if(!values[0].compare("name")) shaderState.name = values[1];
					else if(!values[0].compare("src")) shaderState.src = values[1];
				} else if(state == MaterialState::STATE_ID) {
					if(!values[0].compare("name")) materialState.name = values[1];
					else if(!values[0].compare("shader")) materialState.shader = values[1];
				} else if(state == TextureState::STATE_ID) {
					if(!values[0].compare("name")) textureState.name = values[1];
					else if(!values[0].compare("src")) textureState.src = values[1];
					else if(!values[0].compare("format")) textureState.setFormat(values[1]);
				}
			}
		}
	}

	file.close();
}

