#include "Akila/graphics/Mesh.hpp"

using namespace Akila;

Mesh::Mesh() {

}

void Mesh::addVBO(const std::shared_ptr<Akila::VBO> &vbo) {
	vbos.push_back(vbo);
}

void Mesh::prepare() {
	vao = std::make_shared<Akila::VAO>();
	vao->bind();
	for(auto &vbo : vbos) {
		vao->registerVBO(vbo.get());
	}
	vao->unbind();
}

void Mesh::draw() const {
	vao->draw();
}
