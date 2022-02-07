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



void ResourceFileLoader::fillResourcePool(ResourcePool *rp, std::string const &path, std::function<void()> callback) {
	json file;

	try {
		std::ifstream{FileSystem::path(path)} >> file;
	} catch(const std::exception &) {
		std::cerr << "Resource list loading error : can't read " << path << std::endl;
		return;
	}

	if(file["shaders"].is_array()) for(json &shaderFile : file["shaders"]) {
		if(!shaderFile["name"].is_string() || !shaderFile["src"].is_string()) continue;

		Shader *shader = ShaderLoader::create(shaderFile["src"]);
		rp->shaders.set(shaderFile["name"], shader);

		shader->bind();

		json unifF = shaderFile["uniforms-f"];
		if(unifF.is_object()) for(auto &it : unifF.items()) {
			LOG(it.key());
		}
		//*/
		
		/*/
		for(int i = 0; i < shaderState.uniformsValues.size(); ++i) {
			unsigned int uid = shader->getUniformId(shaderState.unifNames[i]);

			if(shaderState.unifIsInt[i]) {
				shader->sendRawInt(uid, shaderState.uniformsValues[i].values.data(), (int)shaderState.uniformsValues[i].values.size());
			} else {
				shader->sendRawFloat(uid, shaderState.uniformsValues[i].values.data(), (int)shaderState.uniformsValues[i].values.size());
			}
		}//*/

		//rp->renderer->affectUBOToShader(shader.get());
	}
}
