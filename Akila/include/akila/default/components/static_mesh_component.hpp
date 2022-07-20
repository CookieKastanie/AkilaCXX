#pragma once

#include "akila/memory/ref.hpp"
#include "akila/default/resources/static_mesh.hpp"

namespace akila {
	class StaticMeshCompoent {
	public:
		StaticMeshCompoent() = default;
		StaticMeshCompoent(Ref<StaticMesh> const &mesh);

		void setMesh(Ref<StaticMesh> const &mesh);
		StaticMesh *getMesh() const;

	private:
		Ref<StaticMesh> mesh;
	};
}
