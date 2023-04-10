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

		void addVBO(Ptr<VBO> vbo, unsigned int divisor = 0);
		void setIBO(Ptr<IBO> ibo);
		void prepare();

		void draw() const override;
		void drawInstanced(int instanceCount) const;

	private:
		struct VBOAttr {
			Ptr<VBO> vbo;
			unsigned int divisor;
		};

		VAO vao;
		std::vector<VBOAttr> vbos;
		Ptr<IBO> ibo;
	};
}
