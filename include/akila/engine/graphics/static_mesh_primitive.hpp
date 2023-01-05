#pragma once

#include "akila/engine/graphics/static_mesh.hpp"

namespace akila {
	class StaticMeshPrimitive {
	public:
		static StaticMesh *screenTriangle();
		static StaticMesh *quad(float scale = 1.f);
		static StaticMesh *cube(float scale = .5f);
		static StaticMesh *invertedCube(float scale = 1.f);
	};
}