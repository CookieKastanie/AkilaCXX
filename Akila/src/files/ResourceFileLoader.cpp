#include "Akila/files/ResourceFileLoader.hpp"

#include "Akila/core/Core.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include "Akila/files/ShaderLoader.hpp"
#include "Akila/files/TextureLoader.hpp"
#include "Akila/files/MeshLoader.hpp"

using namespace Akila;
using nlohmann::json;

TextureBuffer::Format ResourceFileLoader::stringToFormat(std::string const &str) {
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

TextureBuffer::WrapMode ResourceFileLoader::stringToWrapMode(std::string const &str) {
	if(!str.compare("CLAMP_TO_EDGE")) return TextureBuffer::WrapMode::CLAMP_TO_EDGE;
	if(!str.compare("CLAMP_TO_BORDER")) return TextureBuffer::WrapMode::CLAMP_TO_BORDER;
	if(!str.compare("MIRRORED_REPEAT")) return TextureBuffer::WrapMode::MIRRORED_REPEAT;

	//default
	return TextureBuffer::WrapMode::REPEAT;
}

TextureBuffer::FilterMode ResourceFileLoader::stringToFilterMode(std::string const &str) {
	if(!str.compare("NEAREST")) return TextureBuffer::FilterMode::NEAREST;
	if(!str.compare("NEAREST_MIPMAP_NEAREST")) return TextureBuffer::FilterMode::NEAREST_MIPMAP_NEAREST;
	if(!str.compare("LINEAR_MIPMAP_NEAREST")) return TextureBuffer::FilterMode::LINEAR_MIPMAP_NEAREST;
	if(!str.compare("NEAREST_MIPMAP_LINEAR")) return TextureBuffer::FilterMode::NEAREST_MIPMAP_LINEAR;
	if(!str.compare("LINEAR_MIPMAP_LINEAR")) return TextureBuffer::FilterMode::LINEAR_MIPMAP_LINEAR;

	// default
	return TextureBuffer::FilterMode::LINEAR;
}

void affectUniformI(Shader *shader, json &unifIFile) {
	if(unifIFile.is_object()) for(auto &item : unifIFile.items()) {
		unsigned int uid = shader->getUniformId(item.key());
		json &values = item.value();
		if(values.is_array()) {
			std::size_t size = values.size();
			if(size > 4 || size <= 0) continue;

			std::vector<int> ints(size);
			int index = 0;
			for(json &v : values) {
				if(v.is_number_integer()) ints[index] = v;
				++index;
			}

			shader->sendRawInt(uid, ints.data(), (int)size);
		} else if(values.is_number_integer()) {
			int v[1]{values};
			shader->sendRawInt(uid, v, 1);
		}
	}
}

void affectUniformF(Shader *shader, json &unifFFile) {
	if(unifFFile.is_object()) for(auto &item : unifFFile.items()) {
		unsigned int uid = shader->getUniformId(item.key());
		json &values = item.value();
		if(values.is_array()) {
			std::size_t size = values.size();
			if(size > 4 || size <= 0) continue;

			std::vector<float> floats(size);
			int index = 0;
			for(json &v : values) {
				if(v.is_number()) floats[index] = v;
				++index;
			}

			shader->sendRawFloat(uid, floats.data(), (int)size);
		} else if(values.is_number()) {
			float v[1]{values};
			shader->sendRawFloat(uid, v, 1);
		}
	}
}
int ResourceFileLoader::count = 0;
std::function<void()> ResourceFileLoader::callback = []() {};
std::function<void()> ResourceFileLoader::countCB = []() {
	if(--count <= 0) callback();
};

void ResourceFileLoader::fillResourcePool(ResourcePool *rp, std::string const &path, std::function<void()> const &cb) {
	json file;

	try {
		std::ifstream{FileSystem::path(path)} >> file;
	} catch(const std::exception &) {
		std::cerr << "Resource list loading error : can't read " << path << std::endl;
		return;
	}

	count = 0;
	callback = cb;

	//// Shaders
	if(file["shaders"].is_array()) for(json &shaderFile : file["shaders"]) {
		if(!shaderFile["name"].is_string() || !shaderFile["src"].is_string()) continue;

		Shader *shader = ShaderLoader::create(shaderFile["src"]);
		rp->shaders.set(shaderFile["name"], shader);

		shader->bind();
		affectUniformI(shader, shaderFile["uniforms-i"]);
		affectUniformF(shader, shaderFile["uniforms-f"]);
		Core::renderer->affectUBOToShader(shader);
	}

	//// Meshs
	if(file["meshs"].is_array()) for(json &meshFile : file["meshs"]) {
		if(!meshFile["name"].is_string() || !meshFile["src"].is_string()) continue;
		
		Mesh *mesh = new Mesh{};
		++count;
		MeshLoader::obj(mesh, meshFile["src"], countCB);
		rp->meshs.set(meshFile["name"], mesh);
	}

	//// Textures
	if(file["textures"].is_array()) for(json &textureFile : file["textures"]) {
		if(!textureFile["name"].is_string() || !textureFile["src"].is_string()) continue;

		Texture *texture;
		if(textureFile["format"].is_string())
			texture = new Texture{ResourceFileLoader::stringToFormat(textureFile["format"])};
		
		texture = new Texture{};

		Texture::Parameters params{};
		if(textureFile["wrapS"].is_string())
			params.wrapS = ResourceFileLoader::stringToWrapMode(textureFile["wrapS"]);

		if(textureFile["wrapT"].is_string())
			params.wrapT = ResourceFileLoader::stringToWrapMode(textureFile["wrapT"]);

		if(textureFile["wrapR"].is_string())
			params.wrapR = ResourceFileLoader::stringToWrapMode(textureFile["wrapR"]);

		if(textureFile["magFilter"].is_string())
			params.magFilter = ResourceFileLoader::stringToFilterMode(textureFile["magFilter"]);

		if(textureFile["minFilter"].is_string())
			params.minFilter = ResourceFileLoader::stringToFilterMode(textureFile["minFilter"]);

		bool mips = false;
		if(textureFile["mips"].is_boolean()) mips = textureFile["mips"];

		++count;
		TextureLoader::color(texture, textureFile["src"], [=]() {
			texture->setParameters(params);
			if(mips) texture->generateMipmap();
			countCB();
		});
		rp->textures.set(textureFile["name"], texture);
	}

	//// Materials
	if(file["materials"].is_array()) for(json &materialFile : file["materials"]) {

	}

	if(count <= 0) callback();
}
