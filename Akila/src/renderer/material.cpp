#include "akila/renderer/material.hpp"
#include "akila/renderer/shader_preproc.hpp"

using namespace akila;

MaterialInstance::MaterialInstance(Ref<Material> material): material{material} {

}

void akila::MaterialInstance::write(std::string const &name, int data) {
	writeRaw<int>(material->uniforms[name].offset, &data, 1);
}

/////////////////////////////

Material::Material(std::string const &shaderTxt) {
	ShaderPreProc::ShaderSources sources;
	ShaderPreProc::process(shaderTxt, sources);
	shader.build(sources.vertexShader, sources.fragmentShader, sources.geometryShader);

	glGenVertexArrays(1, &vb);
}
