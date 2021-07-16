#pragma once

#include "Akila/graphics/gl/Buffer.hpp"
#include "Akila/graphics/gl/VAO.hpp"
#include <vector>
#include <memory>

namespace Akila {
	class Mesh {
	private:
		friend class Renderer;

		std::shared_ptr<Akila::VAO> vao;
		std::vector<std::shared_ptr<Akila::VBO>> vbos;

	public:
		Mesh();

		void addVBO(const std::shared_ptr<Akila::VBO> &vbo);
		void prepare();

		void draw() const;
	};
}
