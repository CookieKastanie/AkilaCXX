#pragma once

#include <akila/akila.hpp>
#include <akila/default/resources.hpp>

struct MeshComponent {
	akila::Ref<akila::StaticMesh> mesh;
	akila::Material material;
};
