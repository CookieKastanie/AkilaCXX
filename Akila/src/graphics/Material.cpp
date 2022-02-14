#include "Akila/graphics/Material.hpp"

using namespace Akila;

int Material::lastId = -1;
int Material::currentUsed = -1;

Material::Material() {
	id = ++lastId;
}

int Material::getId() const {
	return id;
}

Shader *Material::getShader() const {
	return shader.raw();
}

void Material::setShader(Ref<Shader> const &shader) {
	this->shader = shader;
}

void Material::addUniformValue(const UniformValue &uv, bool isInts) {
	if(isInts) uniformsInts.push_back(uv);
	else uniformsFloat.push_back(uv);
}

void Material::sendUniforms() const {
	for(auto uv : uniformsFloat) {
		shader->sendRawFloat(uv.uid, uv.values.data(), (int)uv.values.size());
	}

	for(auto uv : uniformsInts) {
		shader->sendRawInt(uv.uid, uv.values.data(), (int)uv.values.size());
	}
}

void Material::addTextureBinding(TextureBinding const &tb) {
	textures.push_back(tb);
}

void Material::bindTextures() const {
	for(auto &tex : textures) {
		tex.textureBuffer->bind(tex.unit);
	}
}
