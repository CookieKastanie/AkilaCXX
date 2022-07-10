#include "akila/renderer/material.hpp"
#include "akila/resource/resources.hpp"

using namespace akila;

Material::Material(Ref<Shader> shader): shader{shader} {
	uniformData.resize(shader->getTotalByteCount(), 0);
}

Material::Material(std::string const &shaderTxt) {
	shader = Resources::create<Shader>("simpleShader");
	shader->build(shaderTxt);

	uniformData.resize(shader->getTotalByteCount(), 0);
}

void Material::use(std::string const &name) {
	if(!shader->uniformExist(name)) {
		std::cerr << "uniform : " << name << " is not present in shader" << std::endl;
		return;
	}

	usedUniforms.push_back(&shader->getUniforminfos(name));
	uniforms[name] = &shader->getUniforminfos(name);

	// tri pour avoir une lecture sequenciel dans la methode send
	std::sort(usedUniforms.begin(), usedUniforms.end(), [](UniformInfos *i1, UniformInfos *i2) {
		return i1->byteOffset < i2->byteOffset;
	});
}

void Material::write(std::string const &name, int data) {
	writeRaw<int>(uniforms.at(name), &data, 1);
}

void Material::write(std::string const &name, float data) {
	writeRaw<float>(uniforms.at(name), &data, 1);
}

void Material::write(std::string const &name, Vec2 const &data) {
	writeRaw<Vec2>(uniforms.at(name), &data, 1);
}

void Material::write(std::string const &name, std::vector<Vec2> const &data) {
	writeRaw<Vec2>(uniforms.at(name), data.data(), data.size());
}

void Material::write(std::string const &name, Vec3 const &data) {
	writeRaw<Vec3>(uniforms.at(name), &data, 1);
}

void Material::write(std::string const &name, std::vector<Vec3> const &data) {
	writeRaw<Vec3>(uniforms.at(name), data.data(), data.size());
}

void Material::write(std::string const &name, Vec4 const &data) {
	writeRaw<Vec4>(uniforms.at(name), &data, 1);
}

void Material::write(std::string const &name, std::vector<Vec4> const &data) {
	writeRaw<Vec4>(uniforms.at(name), data.data(), data.size());
}

void Material::write(std::string const &name, Mat4 const &data) {
	writeRaw<Mat4>(uniforms.at(name), &data, 1);
}

void Material::write(std::string const &name, std::vector<Mat4> const &data) {
	writeRaw<Mat4>(uniforms.at(name), data.data(), data.size());
}

void Material::send() {
	if(!shader->isBinded()) shader->bind();

	for(UniformInfos *infos : usedUniforms) {
		shader->sendRaw(*infos, uniformData.data() + infos->byteOffset);
	}
}
