#pragma once

#include "akila/core/memory/ref.hpp"
#include "akila/engine/graphics/mesh.hpp"
#include "akila/engine/graphics/material.hpp"

namespace akila {
	struct MeshComponent {
		Ref<Mesh> mesh;
		MaterialInstance material;
	};
}
