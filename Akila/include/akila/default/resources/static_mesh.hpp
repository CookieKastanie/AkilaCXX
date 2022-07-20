#pragma once

#include "akila/memory/ptr.hpp"
#include "akila/renderer/buffer_object.hpp"
#include "akila/renderer/vao.hpp"
#include <vector>

namespace akila {
	class StaticMesh {
	public:
		enum Attributes: unsigned int {
			POSITION = 0,
			UV = 1,
			NORMAL = 2,
			TANGENT = 3,
			COLOR = 4
		};

		StaticMesh();

		void addVBO(Ptr<VBO> const &vbo);
		void setIBO(Ptr<IBO> const &ibo);
		void prepare();

		void draw() const;

	private:
		Ptr<VAO> vao;
		std::vector<Ptr<VBO>> vbos;
		Ptr<IBO> ibo;
	};
}
