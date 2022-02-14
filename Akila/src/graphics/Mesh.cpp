#include "Akila/graphics/Mesh.hpp"

using namespace Akila;

Mesh::Mesh() {
	vao = createPtr<VAO>();
}

void Mesh::addVBO(const Ptr<VBO> &vbo) {
	vbos.push_back(vbo);
}

void Mesh::prepare() {
	vao->bind();
	for(auto &vbo : vbos) {
		vao->registerVBO(vbo.get());
	}
	vao->unbind();
}

void Mesh::draw() const {
	vao->draw();
}
