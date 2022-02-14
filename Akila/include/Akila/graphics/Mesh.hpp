#pragma once

#include "Akila/graphics/gl/Buffer.hpp"
#include "Akila/graphics/gl/VAO.hpp"
#include <vector>
#include "Akila/core/Memory.hpp"

namespace Akila {
	class Mesh {
	private:
		friend class Renderer;

		Ptr<VAO> vao;
		std::vector<Ptr<VBO>> vbos;

	public:
		Mesh();

		void addVBO(const Ptr<VBO> &vbo);
		void prepare();

		void draw() const;
	};
}
