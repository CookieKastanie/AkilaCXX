#include "akila/engine/loaders/material_parser.hpp"
#include "akila/core/resources/resources.hpp"
#include "akila/core/rhi/texture.hpp"

using namespace akila;

void MaterialParser::populateUniforms(JSON &json, internal::MaterialContainer *mat) {
	if(json["uniforms"].is_object() == false) return;

	auto const &uniformInfosList = mat->getUniformInfos();

	for(auto &item : json["uniforms"].items()) {
		std::size_t i = 0;
		bool found = false;
		for(; i < uniformInfosList.size(); ++i) {
			if(uniformInfosList[i].name == item.key()) {
				found = true;
				break;
			}
		}

		if(found == false) {
			std::cerr << "JSON : " << item.key() << " not in material" << std::endl;
			continue;
		}

		auto &value = item.value();

		UniformInfos const &infos = uniformInfosList[i];

		switch(infos.baseType) {
			case UniformUnderlyingType::FLOAT:
			{
				if(value.is_array()) {
					std::vector<float> data;
					for(auto &v : value) {
						if(v.is_number()) data.push_back(v);
						else break;
					}
					mat->write(infos.name, data);
				} else if(value.is_number()) {
					float data = value;
					mat->write(infos.name, data);
				}
				break;
			}
			
			case UniformUnderlyingType::INT:
			case UniformUnderlyingType::SAMPLER:
			{
				if(value.is_array()) {
					std::vector<int> data;
					for(auto &v : value) {
						if(v.is_number_integer()) data.push_back(v);
						else break;
					}
					mat->write(infos.name, data);
				} else if(value.is_number_integer()) {
					int data = value;
					mat->write(infos.name, data);
				}
				break;
			}

			case UniformUnderlyingType::UINT:
			{
				if(value.is_array()) {
					std::vector<unsigned int> data;
					for(auto &v : value) {
						if(v.is_number_unsigned()) data.push_back(v);
						else break;
					}
					mat->write(infos.name, data);
				} else if(value.is_number_unsigned()) {
					unsigned int data = value;
					mat->write(infos.name, data);
				}
				break;
			}

			case UniformUnderlyingType::BOOL:
			{
				if(value.is_array()) {
					std::vector<int> data;
					for(auto &v : value) {
						if(v.is_boolean()) data.push_back(static_cast<bool>(v));
						else break;
					}
					mat->write(infos.name, data);
				} else if(value.is_boolean()) {
					int data = static_cast<bool>(value);
					mat->write(infos.name, data);
				}
				break;
			}
		}
	}
}

Ref<TextureBuffer> getTexture(std::string const &arrayName, std::string const &name) {
	if(arrayName == "texturecubemap") return Resources::get<TextureCubmap>(name);
	if(arrayName == "texture3d") return Resources::get<Texture3D>(name);
	return Resources::get<Texture2D>(name);
}

void MaterialParser::populateTextures(JSON &json, internal::MaterialContainer *mat) {
	std::array<std::string, 1> acceptedNames = {"texture2d"};

	std::string arrayName = "";
	for(std::string &arrayName : acceptedNames) {
		if(json[arrayName].is_object() == false) {
			continue;
		}
			
		for(auto &item : json[arrayName].items()) {
			auto &value = item.value();
			if(value.is_string() == false) {
				continue;
			}

			std::string textureName = value;
			bool success = mat->affect(item.key(), getTexture(arrayName, textureName));

			if(success == false) {
				std::cerr << "JSON : " << item.key() << " not in material" << std::endl;
				continue;
			}
		}
	}
}
