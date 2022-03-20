#pragma once

#include "Akila/graphics/Mesh.hpp"
#include <functional>
#include <string>

namespace Akila {
	class MeshLoader {
	public:
		static void obj(Mesh *mesh, std::string const &path, std::function<void()> const &callback);
		static void glb(Mesh *mesh, std::string const &path, std::function<void()> const &callback);
	};
}
