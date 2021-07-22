#include "Akila/core/ResourcesBucket.hpp"
#include "Akila/files/FileSystem.hpp"
#include "Akila/files/Loader.hpp"
#include "Akila/graphics/gl/Texture.hpp"
#include "Akila/graphics/ShaderBuilder.hpp"

using namespace Akila;

ResourcesBucket::ResourcesBucket(const std::shared_ptr<Renderer> &renderer): renderer{renderer} {
	defaultShader = std::make_shared<Shader>("void main(){gl_Position=vec4(0.);}", "void main(){gl_FragColor=vec4(1.);}");

	defaultTexture = std::make_shared<Texture>();

	defaultCubeMapTexture = std::make_shared<CubeMapTexture>();

	defaultMaterial = std::make_shared<Material>();
	defaultMaterial->setShader(defaultShader);

	defaultMesh = std::make_shared<Mesh>();
	{
		auto vertex = std::make_shared<Akila::VBO>(2, ShaderBuilder::Attributes::A_POSITION);
		vertex->setData(std::vector<glm::vec2>({
			{-1, -1}, {1, -1}, {1, 1},
			{-1, -1}, {1, 1}, {-1, 1}
		}));

		auto uv = std::make_shared<Akila::VBO>(2, ShaderBuilder::Attributes::A_UV);
		uv->setData(std::vector<glm::vec2>({
			{0, 0}, {1, 0}, {1, 1},
			{0, 0}, {1, 1}, {0, 1}
		}));

		defaultMesh->addVBO(vertex);
		defaultMesh->addVBO(uv);
	}

	defaultMesh->prepare();

	setMesh("akila_triangle", defaultMesh);
}

std::shared_ptr<Shader> &ResourcesBucket::getShader(const std::string &name) {
	auto &&val = shaders[name];
	if(val != nullptr) return val;

	std::cerr << "Shader '" << name << "' does not exist right now" << std::endl;
	return defaultShader;
}

std::shared_ptr<Texture> &ResourcesBucket::getTexture(const std::string &name) {
	auto &&val = textures[name];
	if(val != nullptr) return val;

	std::cerr << "Texture '" << name << "' does not exist right now" << std::endl;
	return defaultTexture;
}

std::shared_ptr<CubeMapTexture> &ResourcesBucket::getCubeMapTexture(const std::string &name) {
	auto &&val = cubeMapTextures[name];
	if(val != nullptr) return val;

	std::cerr << "CubeMap '" << name << "' does not exist right now" << std::endl;
	return defaultCubeMapTexture;
}

std::shared_ptr<Material> &ResourcesBucket::getMaterial(const std::string &name) {
	auto &&val = materials[name];
	if(val != nullptr) return val;
	
	std::cerr << "Material '" << name << "' does not exist right now" << std::endl;
	return defaultMaterial;
}

std::shared_ptr<Mesh> &ResourcesBucket::getMesh(const std::string &name) {
	auto &&val = meshs[name];
	if(val != nullptr) return val;

	std::cerr << "Mesh '" << name << "' does not exist right now" << std::endl;
	return defaultMesh;
}

void ResourcesBucket::setShader(const std::string &name, const std::shared_ptr<Shader> &shader) {
	shaders[name] = shader;
}

void ResourcesBucket::setTexture(const std::string &name, const std::shared_ptr<Texture> &texture) {
	textures[name] = texture;
}

void ResourcesBucket::setCubeMapTexture(const std::string &name, const std::shared_ptr<CubeMapTexture> &cubeMapTexture) {
	cubeMapTextures[name] = cubeMapTexture;
}

void ResourcesBucket::setMaterial(const std::string &name, const std::shared_ptr<Material> &material) {
	materials[name] = material;
}

void ResourcesBucket::setMesh(const std::string &name, const std::shared_ptr<Mesh> &mesh) {
	meshs[name] = mesh;
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
		if(!str.compare("RGBA")) return TextureBuffer::Format::RGBA;
		if(!str.compare("SRGB")) return TextureBuffer::Format::SRGB;
		if(!str.compare("SRGB_ALPHA")) return TextureBuffer::Format::SRGB_ALPHA;
		if(!str.compare("RGB16F")) return TextureBuffer::Format::RGB16F;
		if(!str.compare("RGBA16F")) return TextureBuffer::Format::RGBA16F;

		//default
		return TextureBuffer::Format::RGB;
	}

	static TextureBuffer::WrapMode stringToWrapMode(const std::string &str) {
		if(!str.compare("CLAMP_TO_EDGE")) return TextureBuffer::WrapMode::CLAMP_TO_EDGE;
		if(!str.compare("CLAMP_TO_BORDER")) return TextureBuffer::WrapMode::CLAMP_TO_BORDER;
		if(!str.compare("MIRRORED_REPEAT")) return TextureBuffer::WrapMode::MIRRORED_REPEAT;

		//default
		return TextureBuffer::WrapMode::REPEAT;
	}

	static TextureBuffer::FilterMode stringToFilterMode(const std::string &str) {
		if(!str.compare("NEAREST")) return TextureBuffer::FilterMode::NEAREST;
		if(!str.compare("NEAREST_MIPMAP_NEAREST")) return TextureBuffer::FilterMode::NEAREST_MIPMAP_NEAREST;
		if(!str.compare("LINEAR_MIPMAP_NEAREST")) return TextureBuffer::FilterMode::LINEAR_MIPMAP_NEAREST;
		if(!str.compare("NEAREST_MIPMAP_LINEAR")) return TextureBuffer::FilterMode::NEAREST_MIPMAP_LINEAR;
		if(!str.compare("LINEAR_MIPMAP_LINEAR")) return TextureBuffer::FilterMode::LINEAR_MIPMAP_LINEAR;
		
		// default
		return TextureBuffer::FilterMode::LINEAR;
	}

	static bool stringToBool(const std::string &str) {
		return !str.compare("true");
	}
};


struct MeshState {
	static const int STATE_ID = 2;

	std::string name;
	std::string src;

	void clear() {
		name = "none";
		src = "";
	}
};



struct MaterialState {
	static const int STATE_ID = 3;

	std::string name;
	std::string shader;

	std::vector<std::string> unifNames;
	std::vector<bool> unifIsInt;
	std::vector<Material::UniformValue> uniformsValues;

	std::vector<Material::TextureBinding> textureBindings;

	void clear() {
		name = "none";
		shader = "";

		unifNames.clear();
		unifIsInt.clear();
		uniformsValues.clear();

		textureBindings.clear();
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



class LoadingInstance {
public:
	bool fileEnd;
	int fileCount;
	int loadedFileCount;

	std::function<void()> callback;

	LoadingInstance(): fileEnd{false}, fileCount{0}, loadedFileCount{0} {

	}

	void check() {
		if(fileEnd && (fileCount == loadedFileCount)) callback();
	}
};

void ResourcesBucket::loadResourceFile(const std::string &path, const std::function<void()> &cb, TaskManager *taskManger) {
	std::shared_ptr<LoadingInstance> loadingInstance = std::make_shared<LoadingInstance>();
	loadingInstance->callback = cb;

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

		if(state == -1) {
			if(values.size() >= 2) {
				if(!values[1].compare("{")) {
					if(!values[0].compare("shader")) { state = ShaderState::STATE_ID; shaderState.clear(); }
					else if(!values[0].compare("material")) { state = MaterialState::STATE_ID; materialState.clear(); }
					else if(!values[0].compare("texture")) { state = TextureState::STATE_ID; textureState.clear(); }
					else if(!values[0].compare("mesh")) { state = MeshState::STATE_ID; meshState.clear(); }
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
							shader->sendRawInt(uid, shaderState.uniformsValues[i].values.data(), (int)shaderState.uniformsValues[i].values.size());
						} else {
							shader->sendRawFloat(uid, shaderState.uniformsValues[i].values.data(), (int)shaderState.uniformsValues[i].values.size());
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

					for(auto tb : materialState.textureBindings) {
						m->addTextureBinding(tb);
					}
				}

				else if(state == TextureState::STATE_ID) {
					auto t = std::make_shared<Texture>(textureState.format);
					t->setParameters(textureState.parameters);

					loadingInstance->fileCount += 1;
					Loader::asyncTexture(t.get(), textureState.src, textureState.generateMips, [loadingInstance]() -> void {
						loadingInstance->loadedFileCount += 1;
						loadingInstance->check();
					}, taskManger);
					textures.emplace(textureState.name, t);
				}

				else if(state == MeshState::STATE_ID) {
					auto m = std::make_shared<Mesh>();

					loadingInstance->fileCount += 1;
					Loader::asyncMesh(m.get(), meshState.src, [loadingInstance]() -> void {
						loadingInstance->loadedFileCount += 1;
						loadingInstance->check();
					}, taskManger);
					meshs.emplace(meshState.name, m);
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
					else if(!values[0].compare("texture")) {
						std::string d = values[1];
						Loader::splitString(values, d, "=");

						if(values.size() >= 2) {
							Material::TextureBinding tb;
							tb.textureBuffer = getTexture(values[0]);
							tb.unit = (unsigned int)stoi(values[1]);
							materialState.textureBindings.push_back(tb);
						}
					}
					else if(!values[0].compare("cubemap")) {
						std::string d = values[1];
						Loader::splitString(values, d, "=");

						if(values.size() >= 2) {
							Material::TextureBinding tb;
							tb.textureBuffer = getCubeMapTexture(values[0]);
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

