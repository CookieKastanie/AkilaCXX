#pragma once

#include "akila/memory/ref.hpp"
#include "akila/default/resources/static_mesh.hpp"

namespace akila {
	struct StaticMeshCompoent {
	public:
		StaticMeshCompoent() = default;
		StaticMeshCompoent(Ref<StaticMesh> const &mesh);

		Ref<StaticMesh> mesh;
	};
}
