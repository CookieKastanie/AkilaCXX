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
	if(isInts) uniformsInt.push_back(uv);
	else uniformsFloat.push_back(uv);
}

void Material::sendUniforms() const {
	for(auto const &uv : uniformsFloat) {
		shader->sendRawFloat(uv.uid, uv.values.data(), (int)uv.values.size());
	}

	for(auto const &uv : uniformsInt) {
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

void Material::copyFrom(Material const *mat) {
	shader = mat->shader;

	uniformsFloat.clear();
	for(UniformValue const &uv : mat->uniformsFloat) {
		UniformValue uvCopy;

		uvCopy.uid = uv.uid;
		uvCopy.values.reserve(uv.values.size());
		for(UniformValueType const &v : uv.values) {
			uvCopy.values.push_back(v);
		}

		addUniformValue(uvCopy);
	}

	uniformsInt.clear();
	for(UniformValue const &uv : mat->uniformsInt) {
		UniformValue uvCopy;

		uvCopy.uid = uv.uid;
		uvCopy.values.reserve(uv.values.size());
		for(UniformValueType const &v : uv.values) {
			uvCopy.values.push_back(v);
		}

		addUniformValue(uvCopy, true);
	}

	textures.clear();
	for(TextureBinding const &tb : mat->textures) {
		addTextureBinding({tb.unit, tb.textureBuffer});
	}
}
