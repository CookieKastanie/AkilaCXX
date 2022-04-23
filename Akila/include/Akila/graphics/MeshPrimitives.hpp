#pragma once

#include "Akila/graphics/Mesh.hpp"

namespace Akila {
	class MeshPrimitives {
	public:
		static Mesh *screenTriangle();
		static Mesh *quad();
		static Mesh *cube();
		static Mesh *invertedCube();
	};
}
