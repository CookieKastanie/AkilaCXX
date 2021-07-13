#include "Akila/graphics/Material.hpp"

using namespace Akila;

Material::Material() {}

Shader *Material::getShader() {
	return shader.get();
}

void Material::setShader(const std::shared_ptr<Shader> &shader) {
	this->shader = shader;
}
