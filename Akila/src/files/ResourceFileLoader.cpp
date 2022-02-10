#include "Akila/files/ResourceFileLoader.hpp"

#include "Akila/core/Core.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include "Akila/files/ShaderLoader.hpp"
#include "Akila/files/TextureLoader.hpp"
#include "Akila/files/MeshLoader.hpp"

using namespace Akila;
using nlohmann::json;

std::vector<Material::UniformValue> readUniform(Shader *shader, json &unifFile, bool isInt = false) {
	std::vector<Material::UniformValue> uvs;

	if(unifFile.is_object()) for(auto &item : unifFile.items()) {
		Material::UniformValue uv{};

		uv.uid = shader->getUniformId(item.key());
		json &values = item.value();

		if(values.is_array()) {
			std::size_t size = values.size();
			if(size > 4 || size <= 0) continue;

			if(isInt) {
				for(json &v : values) {
					if(v.is_number_integer()) {
						Material::UniformValueType val;
						val.i = int(v);
						uv.values.push_back(val);
					}
				}

				uvs.push_back(uv);
			} else {
				for(json &v : values) {
					if(v.is_number()) {
						Material::UniformValueType val;
						val.f = float(v);
						uv.values.push_back(val);
					}
				}

				uvs.push_back(uv);
			}
		} else {
			Material::UniformValueType val;
			if(isInt) {
				if(values.is_number_integer()) {
					val.i = int(values);
					uv.values.push_back(val);

					uvs.push_back(uv);
				}
			} else {
				if(values.is_number()) {
					val.f = float(values);
					uv.values.push_back(val);

					uvs.push_back(uv);
				}
			}
		}
	}

	return uvs;
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
		auto uvis = readUniform(shader, shaderFile["uniforms-i"], true);
		for(auto &uvi : uvis) shader->sendRawInt(uvi.uid, uvi.values.data(), uvi.values.size());
		auto uvfs = readUniform(shader, shaderFile["uniforms-f"], false);
		for(auto &uvf : uvfs) shader->sendRawFloat(uvf.uid, uvf.values.data(), uvf.values.size());

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
			texture = new Texture{TextureLoader::stringToFormat(textureFile["format"])};
		
		texture = new Texture{};

		Texture::Parameters params{};
		if(textureFile["wrapS"].is_string())
			params.wrapS = TextureLoader::stringToWrapMode(textureFile["wrapS"]);

		if(textureFile["wrapT"].is_string())
			params.wrapT = TextureLoader::stringToWrapMode(textureFile["wrapT"]);

		if(textureFile["wrapR"].is_string())
			params.wrapR = TextureLoader::stringToWrapMode(textureFile["wrapR"]);

		if(textureFile["magFilter"].is_string())
			params.magFilter = TextureLoader::stringToFilterMode(textureFile["magFilter"]);

		if(textureFile["minFilter"].is_string())
			params.minFilter = TextureLoader::stringToFilterMode(textureFile["minFilter"]);

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
		if(!materialFile["name"].is_string() || !materialFile["shader"].is_string()) continue;

		Material *material = new Material{};
		rp->materials.set(materialFile["name"], material);

		material->setShader(rp->shaders.get(materialFile["shader"]));

		if(materialFile["uniforms-i"].is_array()) for(json &unifFile : materialFile["uniforms-i"]) {
			auto uvis = readUniform(material->getShader(), materialFile["uniforms-i"], true);
			for(auto &uvi : uvis) material->addUniformValue(uvi, true);
		}

		if(materialFile["uniforms-f"].is_array()) for(json &unifFile : materialFile["uniforms-f"]) {
			auto uvfs = readUniform(material->getShader(), materialFile["uniforms-f"], false);
			for(auto &uvf : uvfs) material->addUniformValue(uvf, false);
		}

		if(materialFile["texture"].is_object()) for(auto &item : materialFile["texture"].items()) {
			if(item.value().is_number_integer()) {
				Material::TextureBinding tb;
				tb.textureBuffer = rp->textures.get<TextureBuffer>(item.key());
				tb.unit = item.value();
				material->addTextureBinding(tb);
			}
		}

		if(materialFile["cubemap"].is_object()) for(auto &item : materialFile["cubemap"].items()) {
			if(item.value().is_number_integer()) {
				Material::TextureBinding tb;
				tb.textureBuffer = rp->cubeMaps.get<TextureBuffer>(item.key());
				tb.unit = item.value();
				material->addTextureBinding(tb);
			}
		}
	}

	if(count <= 0) callback();
}
