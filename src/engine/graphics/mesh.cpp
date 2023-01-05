#include "akila/engine/graphics/mesh.hpp"

using namespace akila;

Mesh::Mesh(): bounds{} {

}

void Mesh::setBounds(Bounds const &b) {
	bounds = b;
}

Mesh::Bounds const &Mesh::getBounds() {
	return bounds;
}
