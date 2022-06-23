#include "akila/renderer/material.hpp"

using namespace akila;

MaterialInstance::MaterialInstance(Material *material): material{material} {

}

/////////////////////////////

Material::Material(std::string const &shaderTxt) {
	//shader preproc...
	//shader.build(vert, frag, geo);
}

void Material::eraseEmptyInstances() {
	instances.remove_if([](RefAnchor<MaterialInstance> const &ra) {
		return !ra.haveReferences();
	});
}

Ref<MaterialInstance> Material::createInstance() {
	auto &ra = instances.emplace_back();
	ra.setValue(new MaterialInstance{this});
	return ra.createReference();
}
