#pragma once

#include "akila/default/resources/static_mesh.hpp"

namespace akila {
	class SaticMeshPrimitives {
	public:
		static StaticMesh *screenTriangle();
		static StaticMesh *quad();
		static StaticMesh *cube();
		static StaticMesh *invertedCube();
	};
}
