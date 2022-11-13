#pragma once

#include "akila/memory/ptr.hpp"
#include "akila/renderer/buffer_object.hpp"
#include "akila/renderer/vao.hpp"
#include "akila/math/math.hpp"
#include <vector>

namespace akila {
	class StaticMesh {
	public:
		enum Attributes: unsigned int {
			POSITION = 0,
			UV = 1,
			NORMAL = 2,
			TANGENT = 3,
			COLOR = 4,
			UV_0 = UV,
			UV_1 = 5,
			UV_2 = 6,
		};

		StaticMesh();

		void addVBO(Ptr<VBO> const &vbo);
		void setIBO(Ptr<IBO> const &ibo);
		void prepare();

		void draw() const;

		Vec3 mins;
		Vec3 maxs;
		float radius;
		float squaredRadius;

	private:
		Ptr<VAO> vao;
		std::vector<Ptr<VBO>> vbos;
		Ptr<IBO> ibo;
	};
}
