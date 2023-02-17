#include "akila/engine/graphics/static_mesh.hpp"

using namespace akila;

StaticMesh::StaticMesh(): Mesh{} {

}

void StaticMesh::addVBO(Ptr<VBO> vbo) {
	vbos.push_back(vbo);
}

void StaticMesh::setIBO(Ptr<IBO> _ibo) {
	ibo = _ibo;
}

void StaticMesh::prepare() {
	vao.bind();
	for(auto &vbo : vbos) {
		vao.registerVBO(vbo.get());
	}
	if(ibo.get() != nullptr) vao.registerIBO(ibo.get());
	vao.unbind();
}

void StaticMesh::draw() const {
	vao.draw();
}
