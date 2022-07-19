#include "akila/default/loaders/uniform_parser.hpp"

using namespace akila;

void Parser::parseUniforms(Shader *shader, JSON &json, std::function<void(std::string const &name, UniformInfos const &infos, void *data, std::size_t byteCount)> onData) {
	if(!json["uniforms"].is_object()) return;

	for(auto &item : json["uniforms"].items()) {
		if(!shader->uniformExist(item.key())) {
			std::cerr << "JSON : " << item.key() << " not in shader" << std::endl;
			continue;
		}

		auto &value = item.value();

		if(!value.is_array()) continue;

		UniformInfos const &infos = shader->getUniforminfos(item.key());

		switch(infos.baseType) {
			case UniformUnderlyingType::FLOAT: {
				std::vector<float> data;
				for(auto &v : value) if(v.is_number()) data.push_back(v);
				onData(item.key(), infos, data.data(), data.size() * sizeof(float));
				break;
			}
			
			case UniformUnderlyingType::INT :
			case UniformUnderlyingType::SAMPLER: {
				std::vector<int> data;
				for(auto &v : value) if(v.is_number_integer()) data.push_back(v);
				onData(item.key(), infos, data.data(), data.size() * sizeof(int));
				break;
			}

			case UniformUnderlyingType::UINT: {
				std::vector<unsigned int> data;
				for(auto &v : value) if(v.is_number_unsigned()) data.push_back(v);
				onData(item.key(), infos, data.data(), data.size() * sizeof(unsigned int));
				break;
			}

			case UniformUnderlyingType::BOOL: {
				std::vector<int> data;
				for(auto &v : value) if(v.is_boolean()) data.push_back(static_cast<bool>(v));
				onData(item.key(), infos, data.data(), data.size() * sizeof(int));
				break;
			}
		}
	}
}



Ref<TextureBuffer> getTexture(std::string const &arrayName, std::string const &name) {
	//if(arrayName == "texture2d") return Resources::get<Texture2D>(name);
	//else
	return Resources::get<Texture2D>(name);
}

void Parser::parseTextureBinds(Shader *shader, JSON &json, std::function<void(Ref<TextureBuffer> texRef, int unit)> onInt, std::function<void(Ref<TextureBuffer> texRef, std::string const &unifName)> onStr) {
	std::array<std::string, 1> acceptedNames = {"texture2d"};

	std::string arrayName = "";
	for(std::string &n : acceptedNames) {
		if(json[n].is_object()) {
			arrayName = n;
			break;
		}
	}
	if(arrayName.empty()) return;

	for(auto &item : json[arrayName].items()) {
		auto &value = item.value();
		if(value.is_number_integer()) {
			onInt(getTexture(arrayName, item.key()), value);
		} else if(value.is_string()) {
			if(!shader->uniformExist(value)) {
				std::cerr << "JSON : " << value << " not in shader" << std::endl;
				return;
			}

			onStr(getTexture(arrayName, item.key()), value);
		}
	}
}
