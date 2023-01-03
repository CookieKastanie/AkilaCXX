#pragma once

#include "akila/engine/graphics/mesh.hpp"
#include "akila/core/rhi/buffer_object.hpp"
#include "akila/core/rhi/vao.hpp"
#include "akila/core/memory/ptr.hpp"
#include <vector>

namespace akila {
	class StaticMesh: public Mesh {
	public:
		StaticMesh();

		void addVBO(Ptr<VBO> vbo);
		void setIBO(Ptr<IBO> ibo);
		void prepare();

		void draw() const override;

	private:
		VAO vao;
		std::vector<Ptr<VBO>> vbos;
		Ptr<IBO> ibo;
	};
}
