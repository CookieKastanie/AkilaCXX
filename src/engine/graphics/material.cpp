#include "akila/engine/graphics/material.hpp"
#include <algorithm>
#include "akila/core/resources/resources.hpp"
#include "akila/core/rhi/texture.hpp"
#include <array>

using namespace akila;

void internal::MaterialContainer::writeRaw(UniformInfos const &infos, void const *data, std::size_t byteCount) {
	std::memcpy(uniformData.data() + infos.byteOffset, data, byteCount);
}


///


// empty memory space used when user read unvalid material memory
static std::array<std::uint8_t, 512> const voidMaterialMemory{0};

Material::Id Material::nextId = 0;
Material::Id Material::lastSended = 0;

Material::Material(): internal::MaterialContainer{}, id{0}, shader{} {

}

Material::Material(
	std::string const &vertexShader,
	std::string const &geometrieShader,
	std::string const &fragmentShader,
	std::set<std::string> const &reservedUniforms):
	internal::MaterialContainer{},
	id{++nextId},
	shader{
		vertexShader,
		geometrieShader,
		fragmentShader
} {

	std::size_t count = 0;
	for(UniformInfos const &infos : shader.retreiveUniformInfos()) {
		count += infos.byteCount;

		if(reservedUniforms.find(infos.name) != reservedUniforms.end()) {
			reservedUniformDescriptors.push_back(infos);
			continue;
		}

		if(infos.baseType == UniformUnderlyingType::SAMPLER) {
			int unit = 0;
			shader.readInt(infos, &unit);
			textures.push_back({infos.name, unit, Resources::get<Texture2D>("__dummy")});
		} else {
			uniformDescriptors.push_back(infos);
		}
	}

	uniformData.resize(count);
	
	for(std::size_t i = 0; i < reservedUniformDescriptors.size(); ++i) {
		UniformInfos const &infos = reservedUniformDescriptors[i];
		reservedUniformsNamesToIndex[infos.name] = i;
	}

	for(std::size_t i = 0; i < uniformDescriptors.size(); ++i) {
		UniformInfos const &infos = uniformDescriptors[i];
		uniformsNamesToIndex[infos.name] = i;
	}

	for(std::size_t i = 0; i < textures.size(); ++i) {
		TextureBinding const &tb = textures[i];
		texturesNamesToIndex[tb.name] = i;
	}
}


#define MATERIAL_WRITE_BODY(X) \
auto it = uniformsNamesToIndex.find(name); \
if(it == uniformsNamesToIndex.end()) return; \
writeRaw(uniformDescriptors[it->second], &data, X);

void Material::write(std::string const &name, int data) {
	MATERIAL_WRITE_BODY(sizeof(int));
}

void Material::write(std::string const &name, float data) {
	MATERIAL_WRITE_BODY(sizeof(float));
}

void Material::write(std::string const &name, Vec2 const &data) {
	MATERIAL_WRITE_BODY(sizeof(Vec2));
}

void Material::write(std::string const &name, std::vector<Vec2> const &data) {
	MATERIAL_WRITE_BODY(data.size() * sizeof(Vec2));
}

void Material::write(std::string const &name, Vec3 const &data) {
	MATERIAL_WRITE_BODY(sizeof(Vec3));
}

void Material::write(std::string const &name, std::vector<Vec3> const &data) {
	MATERIAL_WRITE_BODY(data.size() * sizeof(Vec3));
}

void Material::write(std::string const &name, Vec4 const &data) {
	MATERIAL_WRITE_BODY(sizeof(Vec4));
}

void Material::write(std::string const &name, std::vector<Vec4> const &data) {
	MATERIAL_WRITE_BODY(data.size() * sizeof(Vec4));
}

void Material::write(std::string const &name, Mat4 const &data) {
	MATERIAL_WRITE_BODY(sizeof(Mat4));
}

void Material::affect(std::string const &name, Ref<TextureBuffer> texRef) {
	auto it = texturesNamesToIndex.find(name);
	if(it == texturesNamesToIndex.end()) return;
	textures[it->second].textureBuffer = texRef;
}

void Material::send() {
	if(lastSended != id) {
		shader.bind();
		lastSended = id;
	}

	for(UniformInfos const &infos : uniformDescriptors) {
		shader.send(infos, uniformData.data() + infos.byteOffset);
	}

	for(TextureBinding const &binding : textures) {
		binding.textureBuffer->bind(binding.unit);
	}
}


#define MATERIAL_SEND_RESERVED_BODY(X) \
auto it = reservedUniformsNamesToIndex.find(name); \
if(it == reservedUniformsNamesToIndex.end()) return; \
shader.send(reservedUniformDescriptors[it->second], X);

void Material::sendReserved(std::string const &name, int data) {
	MATERIAL_SEND_RESERVED_BODY(&data);
}

void Material::sendReserved(std::string const &name, float data) {
	MATERIAL_SEND_RESERVED_BODY(&data);
}

void Material::sendReserved(std::string const &name, Vec2 const &data) {
	MATERIAL_SEND_RESERVED_BODY(&data[0]);
}

void Material::sendReserved(std::string const &name, std::vector<Vec2> const &data) {
	MATERIAL_SEND_RESERVED_BODY(data.data());
}

void Material::sendReserved(std::string const &name, Vec3 const &data) {
	MATERIAL_SEND_RESERVED_BODY(&data[0]);
}

void Material::sendReserved(std::string const &name, std::vector<Vec3> const &data) {
	MATERIAL_SEND_RESERVED_BODY(data.data());
}

void Material::sendReserved(std::string const &name, Vec4 const &data) {
	MATERIAL_SEND_RESERVED_BODY(&data[0]);
}

void Material::sendReserved(std::string const &name, std::vector<Vec4> const &data) {
	MATERIAL_SEND_RESERVED_BODY(data.data());
}

void Material::sendReserved(std::string const &name, Mat4 const &data) {
	MATERIAL_SEND_RESERVED_BODY(&data[0]);
}


///


MaterialInstance::MaterialInstance(): internal::MaterialContainer{}, material{Resources::get<Material>("__dummy")} {

}

MaterialInstance::MaterialInstance(Ref<Material> mat): internal::MaterialContainer{}, material{mat} {
	overridedUniforms.resize(material->uniformDescriptors.size(), false);
	overridedTextures.resize(material->textures.size(), false);

	uniformData.resize(material->uniformData.size());
	textures.resize(material->textures.size());
}

void MaterialInstance::send() {
	Shader const &shader = material->shader;
	auto const &uniformDescriptors = material->uniformDescriptors;
	auto *matUniformData = material->uniformData.data();
	auto const &matTextures = material->textures;

	if(Material::lastSended != material->id) {
		shader.bind();
		Material::lastSended = material->id;
	}

	for(std::size_t i = 0; i < overridedUniforms.size(); ++i) {
		UniformInfos const &infos = uniformDescriptors[i];
		if(overridedUniforms[i] != 0) {
			shader.send(infos, uniformData.data() + infos.byteOffset);
		} else {
			shader.send(infos, matUniformData + infos.byteOffset);
		}
	}

	for(std::size_t i = 0; i < overridedTextures.size(); ++i) {
		if(overridedTextures[i] != 0) {
			TextureBinding const &tb = textures[i];
			tb.textureBuffer->bind(tb.unit);
		} else {
			TextureBinding const &tb = matTextures[i];
			tb.textureBuffer->bind(tb.unit);
		}
	}
}

#define MATERIAL_INSTANCE_WRITE_BODY(X) \
auto const &uniformsNamesToIndex = material->uniformsNamesToIndex; \
auto it = uniformsNamesToIndex.find(name); \
if(it == uniformsNamesToIndex.end()) return; \
std::size_t index = it->second; \
overridedUniforms[index] = true; \
writeRaw(material->uniformDescriptors[index], &data, X);

void MaterialInstance::write(std::string const &name, int data) {
	MATERIAL_INSTANCE_WRITE_BODY(sizeof(int));
}

void MaterialInstance::write(std::string const &name, float data) {
	MATERIAL_INSTANCE_WRITE_BODY(sizeof(float));
}

void MaterialInstance::write(std::string const &name, Vec2 const &data) {
	MATERIAL_INSTANCE_WRITE_BODY(sizeof(Vec2));
}

void MaterialInstance::write(std::string const &name, std::vector<Vec2> const &data) {
	MATERIAL_INSTANCE_WRITE_BODY(data.size() * sizeof(Vec2));
}

void MaterialInstance::write(std::string const &name, Vec3 const &data) {
	MATERIAL_INSTANCE_WRITE_BODY(sizeof(Vec3));
}

void MaterialInstance::write(std::string const &name, std::vector<Vec3> const &data) {
	MATERIAL_INSTANCE_WRITE_BODY(data.size() * sizeof(Vec3));
}

void MaterialInstance::write(std::string const &name, Vec4 const &data) {
	MATERIAL_INSTANCE_WRITE_BODY(sizeof(Vec4));
}

void MaterialInstance::write(std::string const &name, std::vector<Vec4> const &data) {
	MATERIAL_INSTANCE_WRITE_BODY(data.size() * sizeof(Vec4));
}

void MaterialInstance::write(std::string const &name, Mat4 const &data) {
	MATERIAL_INSTANCE_WRITE_BODY(sizeof(Mat4));
}

void MaterialInstance::affect(std::string const &name, Ref<TextureBuffer> texRef) {
	auto const &texturesNamesToIndex = material->texturesNamesToIndex;

	auto it = texturesNamesToIndex.find(name);
	if(it == texturesNamesToIndex.end()) return;

	std::size_t index = it->second;
	textures[index].textureBuffer = texRef;
	overridedTextures[index] = true;
}
