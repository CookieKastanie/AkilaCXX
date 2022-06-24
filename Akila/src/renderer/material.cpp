#include "akila/renderer/material.hpp"

using namespace akila;

MaterialInstance::MaterialInstance(Material *material): material{material} {

}

/////////////////////////////

Material::Material(std::string const &shaderTxt) {
	//shader preproc...
	//shader.build(vert, frag, geo);
}

void Material::createInstance() {
	
}
