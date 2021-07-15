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
	return shader.get();
}

void Material::setShader(const std::shared_ptr<Shader> &shader) {
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
