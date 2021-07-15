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

//////////////////////////////////////////////////////////////////////////////////////////////////////////

struct ShaderState {
	static const int STATE_ID = 0;

	std::string name;
	std::string src;

	// initial values
	std::vector<std::string> unifNames;
	std::vector<bool> unifIsInt;
	std::vector<Material::UniformValue> uniformsValues;


	void clear() {
		name = "none";
		src = "";

		unifNames.clear();
		unifIsInt.clear();
		uniformsValues.clear();
	}
};

struct TextureState {
	static const int STATE_ID = 1;

	std::string name;
	std::string src;
	TextureBuffer::Format format;
	TextureBuffer::Parameters parameters;
	bool generateMips;

	void clear() {
		name = "none";
		src = "";
		format = TextureBuffer::Format::RGB;
		parameters = TextureBuffer::Parameters{};
		generateMips = false;
	}

	static TextureBuffer::Format stringToFormat(const std::string &str) {
		if(!str.compare("DEPTH_COMPONENT")) return TextureBuffer::Format::DEPTH_COMPONENT;
		if(!str.compare("DEPTH_STENCIL")) return TextureBuffer::Format::DEPTH_STENCIL;
		if(!str.compare("RED")) return TextureBuffer::Format::RED;
		if(!str.compare("RG")) return TextureBuffer::Format::RG;
		if(!str.compare("RGB")) return TextureBuffer::Format::RGB;
		if(!str.compare("RGBA")) return TextureBuffer::Format::RGBA;
		if(!str.compare("SRGB")) return TextureBuffer::Format::SRGB;
		if(!str.compare("SRGB_ALPHA")) return TextureBuffer::Format::SRGB_ALPHA;
		if(!str.compare("RGB16F")) return TextureBuffer::Format::RGB16F;
		if(!str.compare("RGBA16F")) return TextureBuffer::Format::RGBA16F;
	}

	static TextureBuffer::WrapMode stringToWrapMode(const std::string &str) {
		if(!str.compare("CLAMP_TO_EDGE")) return TextureBuffer::WrapMode::CLAMP_TO_EDGE;
		if(!str.compare("CLAMP_TO_BORDER")) return TextureBuffer::WrapMode::CLAMP_TO_BORDER;
		if(!str.compare("MIRRORED_REPEAT")) return TextureBuffer::WrapMode::MIRRORED_REPEAT;
		if(!str.compare("REPEAT")) return TextureBuffer::WrapMode::REPEAT;
	}

	static TextureBuffer::FilterMode stringToFilterMode(const std::string &str) {
		if(!str.compare("NEAREST")) return TextureBuffer::FilterMode::NEAREST;
		if(!str.compare("LINEAR")) return TextureBuffer::FilterMode::LINEAR;
		if(!str.compare("NEAREST_MIPMAP_NEAREST")) return TextureBuffer::FilterMode::NEAREST_MIPMAP_NEAREST;
		if(!str.compare("LINEAR_MIPMAP_NEAREST")) return TextureBuffer::FilterMode::LINEAR_MIPMAP_NEAREST;
		if(!str.compare("NEAREST_MIPMAP_LINEAR")) return TextureBuffer::FilterMode::NEAREST_MIPMAP_LINEAR;
		if(!str.compare("LINEAR_MIPMAP_LINEAR")) return TextureBuffer::FilterMode::LINEAR_MIPMAP_LINEAR;
	}

	static bool stringToBool(const std::string &str) {
		return !str.compare("true");
	}
};

struct MaterialState {
	static const int STATE_ID = 2;

	std::string name;
	std::string shader;

	std::vector<std::string> unifNames;
	std::vector<bool> unifIsInt;
	std::vector<Material::UniformValue> uniformsValues;

	void clear() {
		name = "none";
		shader = "";

		unifNames.clear();
		unifIsInt.clear();
		uniformsValues.clear();
	}
};

static bool parseUniformValue(Material::UniformValue &uv, std::string &name, bool &isInt, std::string &line) {
	std::vector<std::string> values;
	Loader::splitString(values, line, "=");
	if(values.size() < 2) return false;

	name = values[0];

	if(values[1].size() < 3) return false;

	if(values[1][values[1].size() - 1] != ']') return false;
	values[1].erase(values[1].end() - 1);

	if(values[1][0] == 'i') {
		values[1].erase(values[1].begin());
		isInt = true;
	} else {
		isInt = false;
	}

	if(values[1][0] != '[') return false;
	values[1].erase(values[1].begin());

	if(values[1].size() <= 0) return false;

	std::string s = values[1];
	Loader::splitString(values, s, ",");

	int maxValueCount = 4;
	for(std::string &v : values) {
		Material::UniformValueType data;
		if(isInt) data.i = std::stoi(v);
		else data.f = std::stof(v);
		uv.values.push_back(data);

		if(--maxValueCount == 0) break;
	}

	return uv.values.size() != 0;
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
					auto shader = Loader::shader(shaderState.src);
					shaders.emplace(shaderState.name, shader);

					shader->bind();
					for(int i = 0; i < shaderState.uniformsValues.size(); ++i) {
						unsigned int uid = shader->getUniformId(shaderState.unifNames[i]);

						if(shaderState.unifIsInt[i]) {
							shader->sendRawInt(uid, shaderState.uniformsValues[i].values.data(), shaderState.uniformsValues[i].values.size());
						} else {
							shader->sendRawFloat(uid, shaderState.uniformsValues[i].values.data(), shaderState.uniformsValues[i].values.size());
						}
					}
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

					for(int i = 0; i < materialState.uniformsValues.size(); ++i) {
						materialState.uniformsValues[i].uid = m->getShader()->getUniformId(materialState.unifNames[i]);
						m->addUniformValue(materialState.uniformsValues[i], materialState.unifIsInt[i]);
					}
				}

				else if(state == TextureState::STATE_ID) {
					auto t = std::make_shared<Texture>(textureState.format);
					t->setParameters(textureState.parameters);
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
					else if(!values[0].compare("uniform")) {
						Material::UniformValue uv;
						std::string name;
						bool isInt;
						if(parseUniformValue(uv, name, isInt, values[1])) {
							shaderState.uniformsValues.push_back(uv);
							shaderState.unifNames.push_back(name);
							shaderState.unifIsInt.push_back(isInt);
						}
					}
				} 
				
				else if(state == MaterialState::STATE_ID) {
					if(!values[0].compare("name")) materialState.name = values[1];
					else if(!values[0].compare("shader")) materialState.shader = values[1];
					else if(!values[0].compare("uniform")) {
						Material::UniformValue uv;
						std::string name;
						bool isInt;
						if(parseUniformValue(uv, name, isInt, values[1])) {
							materialState.uniformsValues.push_back(uv);
							materialState.unifNames.push_back(name);
							materialState.unifIsInt.push_back(isInt);
						}
					}
				} 
				
				else if(state == TextureState::STATE_ID) {
					if(!values[0].compare("name")) textureState.name = values[1];
					else if(!values[0].compare("src")) textureState.src = values[1];
					else if(!values[0].compare("format")) textureState.format = TextureState::stringToFormat(values[1]);

					else if(!values[0].compare("wrapS")) textureState.parameters.wrapS = TextureState::stringToWrapMode(values[1]);
					else if(!values[0].compare("wrapT")) textureState.parameters.wrapT = TextureState::stringToWrapMode(values[1]);
					else if(!values[0].compare("wrapR")) textureState.parameters.wrapR = TextureState::stringToWrapMode(values[1]);

					else if(!values[0].compare("minFilter")) textureState.parameters.minFilter = TextureState::stringToFilterMode(values[1]);
					else if(!values[0].compare("magFilter")) textureState.parameters.magFilter = TextureState::stringToFilterMode(values[1]);

					else if(!values[0].compare("mips")) textureState.generateMips = TextureState::stringToBool(values[1]);
				}
			}
		}
	}

	file.close();
}

