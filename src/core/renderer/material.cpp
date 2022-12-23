#include "akila/core/renderer/material.hpp"
#include "akila/core/resource/resources.hpp"

using namespace akila;

Material::Material(Ref<Shader> shader): shader{shader} {
	uniformData.resize(shader->getTotalByteCount(), 0);
}

Material::Material(Ref<Material> other):
	shader{other->shader},
	usedUniforms{other->usedUniforms},
	uniforms{other->uniforms},
	uniformData{other->uniformData},
	textures{other->textures} {

}

Material::Material(Material &&other) noexcept:
	shader{other.shader},
	usedUniforms{other.usedUniforms},
	uniforms{other.uniforms},
	uniformData{other.uniformData},
	textures{other.textures} {

}

Material &Material::operator=(Material &&other) noexcept {
	shader = other.shader;
	usedUniforms = other.usedUniforms;
	uniforms = other.uniforms;
	uniformData = other.uniformData;
	textures = other.textures;

	return *this;
}

Material::Material(Material const &other):
	shader{other.shader},
	usedUniforms{other.usedUniforms},
	uniforms{other.uniforms},
	uniformData{other.uniformData},
	textures{other.textures} {

}

Material &Material::operator=(Material const &other) {
	shader = other.shader;
	usedUniforms = other.usedUniforms;
	uniforms = other.uniforms;
	uniformData = other.uniformData;
	textures = other.textures;

	return *this;
}

Ref<Shader> Material::getShaderRef() const {
	return shader;
}

Shader *Material::getShader() const {
	return shader.raw();
}

Ptr<Material> Material::copy() {
	Ptr<Material> mat = createPtr<Material>(shader);
	mat->textures = textures;
	mat->uniformData = uniformData;
	mat->uniforms = uniforms;
	mat->usedUniforms = usedUniforms;

	return mat;
}

bool Material::use(std::string const &name) {
	if(!shader->uniformExist(name)) {
		std::cerr << "uniform : " << name << " is not present in shader" << std::endl;
		return false;
	}

	if(uniforms.find(name) != uniforms.end()) return true;

	UniformInfos const *infos = &shader->getUniforminfos(name);
	usedUniforms.push_back(infos);
	uniforms[name] = infos;

	// tri pour avoir une lecture sequentiel dans la methode send
	std::sort(usedUniforms.begin(), usedUniforms.end(), [](UniformInfos const *i1, UniformInfos const *i2) {
		return i1->byteOffset < i2->byteOffset;
	});

	return true;
}

inline void Material::writeRaw(UniformInfos const *infos, void const *data, std::size_t byteCount) {
	std::memcpy(uniformData.data() + infos->byteOffset, data, byteCount);
}

void Material::write(std::string const &name, int data) {
	writeRaw(uniforms.at(name), &data, sizeof(int));
}

void Material::write(std::string const &name, float data) {
	writeRaw(uniforms.at(name), &data, sizeof(float));
}

void Material::write(std::string const &name, Vec2 const &data) {
	writeRaw(uniforms.at(name), &data, sizeof(Vec2));
}

void Material::write(std::string const &name, std::vector<Vec2> const &data) {
	writeRaw(uniforms.at(name), data.data(), data.size() * sizeof(Vec2));
}

void Material::write(std::string const &name, Vec3 const &data) {
	writeRaw(uniforms.at(name), &data, sizeof(Vec3));
}

void Material::write(std::string const &name, std::vector<Vec3> const &data) {
	writeRaw(uniforms.at(name), data.data(), data.size() * sizeof(Vec3));
}

void Material::write(std::string const &name, Vec4 const &data) {
	writeRaw(uniforms.at(name), &data, sizeof(Vec4));
}

void Material::write(std::string const &name, std::vector<Vec4> const &data) {
	writeRaw(uniforms.at(name), data.data(), data.size() * sizeof(Vec4));
}

void Material::write(std::string const &name, Mat4 const &data) {
	writeRaw(uniforms.at(name), &data, sizeof(Mat4));
}

void Material::write(std::string const &name, std::vector<Mat4> const &data) {
	writeRaw(uniforms.at(name), data.data(), data.size() * sizeof(Mat4));
}

void Material::affect(Ref<TextureBuffer> const &texRef, int unit) {
	textures.push_back({unit, texRef});
}

void Material::affect(Ref<TextureBuffer> const &texRef, std::string const &name) {
	if(!shader->uniformExist(name)) {
		std::cerr << "uniform " << name << " do not exist" << std::endl;
		return;
	}

	UniformInfos const &infos = shader->getUniforminfos(name);
	if(infos.baseType != UniformUnderlyingType::SAMPLER) {
		std::cerr << "uniform " << name << " is not a sampler" << std::endl;
		return;
	}

	int unit = 0;
	if(uniforms.find(name) != uniforms.end()) {
		unit = *(uniformData.data() + infos.byteOffset);
	} else {
		shader->readInt(name, &unit);
	}

	textures.push_back({unit, texRef});
}

void Material::send() {
	if(!shader->isBinded()) shader->bind();

	for(UniformInfos const *infos : usedUniforms) {
		shader->sendRaw(*infos, uniformData.data() + infos->byteOffset);
	}

	for(TextureBinding const &binding : textures) {
		binding.textureBuffer->bind(binding.unit);
	}
}
