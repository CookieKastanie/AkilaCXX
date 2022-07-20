#include "akila/default/components/static_mesh_component.hpp"

using namespace akila;

StaticMeshCompoent::StaticMeshCompoent(Ref<StaticMesh> const &mesh): mesh{mesh} {}

void StaticMeshCompoent::setMesh(Ref<StaticMesh> const &m) {
	mesh = m;
}

StaticMesh *StaticMeshCompoent::getMesh() const {
	return mesh;
}
