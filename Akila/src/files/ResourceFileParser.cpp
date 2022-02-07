#include "Akila/files/ResourceFileParser.hpp"
/*/
#include "Akila/files/FileSystem.hpp"
#include "Akila/files/Loader.hpp"
#include "Akila/graphics/gl/Texture.hpp"
#include "Akila/graphics/ShaderBuilder.hpp"

using namespace Akila;

ResourceFileParser::ResourceFileParser(ResourcePool *rp, const std::function<void()> &onFinish, TaskManager *taskManager):
	rp{rp}, taskManager{taskManager}, onFinish{onFinish} {}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ResourceFileParser::ShaderState::clear() {
	name = "none";
	src = "";

	unifNames.clear();
	unifIsInt.clear();
	uniformsValues.clear();
};

void ResourceFileParser::TextureState::clear() {
	name = "none";
	src = "";
	format = TextureBuffer::Format::RGB;
	parameters = TextureBuffer::Parameters{};
	generateMips = false;
}

TextureBuffer::Format ResourceFileParser::TextureState::stringToFormat(const std::string &str) {
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

TextureBuffer::WrapMode ResourceFileParser::TextureState::stringToWrapMode(const std::string &str) {
	if(!str.compare("CLAMP_TO_EDGE")) return TextureBuffer::WrapMode::CLAMP_TO_EDGE;
	if(!str.compare("CLAMP_TO_BORDER")) return TextureBuffer::WrapMode::CLAMP_TO_BORDER;
	if(!str.compare("MIRRORED_REPEAT")) return TextureBuffer::WrapMode::MIRRORED_REPEAT;

	//default
	return TextureBuffer::WrapMode::REPEAT;
}

TextureBuffer::FilterMode ResourceFileParser::TextureState::stringToFilterMode(const std::string &str) {
	if(!str.compare("NEAREST")) return TextureBuffer::FilterMode::NEAREST;
	if(!str.compare("NEAREST_MIPMAP_NEAREST")) return TextureBuffer::FilterMode::NEAREST_MIPMAP_NEAREST;
	if(!str.compare("LINEAR_MIPMAP_NEAREST")) return TextureBuffer::FilterMode::LINEAR_MIPMAP_NEAREST;
	if(!str.compare("NEAREST_MIPMAP_LINEAR")) return TextureBuffer::FilterMode::NEAREST_MIPMAP_LINEAR;
	if(!str.compare("LINEAR_MIPMAP_LINEAR")) return TextureBuffer::FilterMode::LINEAR_MIPMAP_LINEAR;

	// default
	return TextureBuffer::FilterMode::LINEAR;
}

bool ResourceFileParser::TextureState::stringToBool(const std::string &str) {
	return !str.compare("true");
}

void ResourceFileParser::MeshState::clear() {
	name = "none";
	src = "";
};

void ResourceFileParser::MaterialState::clear() {
	name = "none";
	shader = "";

	unifNames.clear();
	unifIsInt.clear();
	uniformsValues.clear();

	textureBindings.clear();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ResourceFileParser::LoadingInstance::LoadingInstance():
	fileEnd{false}, fileCount{0}, loadedFileCount{0} {}

void ResourceFileParser::LoadingInstance::check() {
	if(fileEnd && (fileCount == loadedFileCount)) callback();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ResourceFileParser::createShader(const ShaderState &shaderState) {
	auto shader = Loader::shader(shaderState.src);
	rp->setShader(shaderState.name, shader);

	shader->bind();
	for(int i = 0; i < shaderState.uniformsValues.size(); ++i) {
		unsigned int uid = shader->getUniformId(shaderState.unifNames[i]);

		if(shaderState.unifIsInt[i]) {
			shader->sendRawInt(uid, shaderState.uniformsValues[i].values.data(), (int)shaderState.uniformsValues[i].values.size());
		} else {
			shader->sendRawFloat(uid, shaderState.uniformsValues[i].values.data(), (int)shaderState.uniformsValues[i].values.size());
		}
	}

	rp->renderer->affectUBOToShader(shader.get());
}

void ResourceFileParser::createMaterial(MaterialState &materialState) {
	auto m = std::make_shared<Material>();
	m->setShader(rp->shaders[materialState.shader]);
	rp->setMaterial(materialState.name, m);

	if(m->getShader() == nullptr) {
		std::cerr << materialState.name << " material don't have shader program !" << std::endl;
		m->setShader(rp->defaultShader);
	}

	for(int i = 0; i < materialState.uniformsValues.size(); ++i) {
		materialState.uniformsValues[i].uid = m->getShader()->getUniformId(materialState.unifNames[i]);
		m->addUniformValue(materialState.uniformsValues[i], materialState.unifIsInt[i]);
	}

	for(auto tb : materialState.textureBindings) {
		m->addTextureBinding(tb);
	}
}

void ResourceFileParser::createTexture(const TextureState &textureState) {
	auto t = std::make_shared<Texture>(textureState.format);
	t->setParameters(textureState.parameters);

	loadingInstance->fileCount += 1;
	std::shared_ptr<LoadingInstance> selfLodaingInstance = loadingInstance;
	Loader::asyncTexture(t.get(), textureState.src, textureState.generateMips, [selfLodaingInstance]() -> void {
		selfLodaingInstance->loadedFileCount += 1;
		selfLodaingInstance->check();
	}, taskManager);

	rp->setTexture(textureState.name, t);
}

void ResourceFileParser::createMesh(const MeshState &meshState) {
	auto m = std::make_shared<Mesh>();

	loadingInstance->fileCount += 1;
	std::shared_ptr<LoadingInstance> selfLodaingInstance = loadingInstance;
	Loader::asyncMesh(m.get(), meshState.src, [selfLodaingInstance]() -> void {
		selfLodaingInstance->loadedFileCount += 1;
		selfLodaingInstance->check();
	}, taskManager);

	rp->setMesh(meshState.name, m);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ResourceFileParser::fromFile(const std::string &path) {
	loadingInstance = std::make_shared<LoadingInstance>();
	loadingInstance->callback = onFinish;

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
	MeshState meshState;

	std::string line;
	while(std::getline(file, line)) {
		std::vector<std::string> values;
		Loader::splitString(values, line, " ");
		for(auto &s : values) Loader::trimString(s);

		// changement d'etat
		if(state == -1) {
			if(values.size() >= 2) {
				if(!values[1].compare("{")) {
					if(!values[0].compare("shader")) {
						state = ShaderState::STATE_ID; shaderState.clear();
					} else if(!values[0].compare("material")) {
						state = MaterialState::STATE_ID; materialState.clear();
					} else if(!values[0].compare("texture")) {
						state = TextureState::STATE_ID; textureState.clear();
					} else if(!values[0].compare("mesh")) {
						state = MeshState::STATE_ID; meshState.clear();
					}
				}
			}

			continue;
		}

		// creation d'un objet
		if(values.size() > 0) {
			if(!values[0].compare("}")) {

				if(state == ShaderState::STATE_ID) {
					createShader(shaderState);
				} else if(state == MaterialState::STATE_ID) {
					createMaterial(materialState);
				} else if(state == TextureState::STATE_ID) {
					createTexture(textureState);
				} else if(state == MeshState::STATE_ID) {
					createMesh(meshState);
				}

				state = -1;
				continue;
			}


			////////////////////////////////////


			line = "";
			for(auto v : values) line += v;

			Loader::splitString(values, line, ":");

			// affectation valeur etat
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
					} else if(!values[0].compare("texture")) {
						std::string d = values[1];
						Loader::splitString(values, d, "=");

						if(values.size() >= 2) {
							Material::TextureBinding tb;
							tb.textureBuffer = rp->getTexture(values[0]);
							tb.unit = (unsigned int)stoi(values[1]);
							materialState.textureBindings.push_back(tb);
						}
					} else if(!values[0].compare("cubemap")) {
						std::string d = values[1];
						Loader::splitString(values, d, "=");

						if(values.size() >= 2) {
							Material::TextureBinding tb;
							tb.textureBuffer = rp->getCubeMapTexture(values[0]);
							tb.unit = (unsigned int)stoi(values[1]);
							materialState.textureBindings.push_back(tb);
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

				else if(state == MeshState::STATE_ID) {
					if(!values[0].compare("name")) meshState.name = values[1];
					else if(!values[0].compare("src")) meshState.src = values[1];
				}
			}
		}
	}

	file.close();

	loadingInstance->fileEnd = true;
	loadingInstance->check();
}
//*/
