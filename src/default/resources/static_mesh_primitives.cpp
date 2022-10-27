#include "akila/default/resources/static_mesh_primitives.hpp"

using namespace akila;

StaticMesh *StaticMeshPrimitives::screenTriangle() {
	StaticMesh *mesh = new StaticMesh{};

	auto vertex = createPtr<VBO>(2, StaticMesh::Attributes::POSITION);
	vertex->setData(std::vector<Vec2>({
		{3, 1}, {-1, 1}, {-1, -3}
	}));

	auto uv = createPtr<VBO>(2, StaticMesh::Attributes::UV);
	uv->setData(std::vector<Vec2>({
		{2, 1}, {0, 1}, {0, -1}
	}));

	mesh->addVBO(vertex);
	mesh->addVBO(uv);

	mesh->prepare();

	return mesh;
}

StaticMesh *StaticMeshPrimitives::quad() {
	StaticMesh *mesh = new StaticMesh{};

	auto vertex = createPtr<VBO>(2, StaticMesh::Attributes::POSITION);
	vertex->setData(std::vector<Vec2>({
		{-1, -1}, {1, -1}, {1, 1},
		{-1, -1}, {1, 1}, {-1, 1}
	}));

	auto uv = createPtr<VBO>(2, StaticMesh::Attributes::UV);
	uv->setData(std::vector<Vec2>({
		{0, 0}, {1, 0}, {1, 1},
		{0, 0}, {1, 1}, {0, 1}
	}));

	auto normal = createPtr<VBO>(3, StaticMesh::Attributes::NORMAL);
	normal->setData(std::vector<Vec3>({
		{0, 0, 1}, {0, 0, 1}, {0, 0, 1},
		{0, 0, 1}, {0, 0, 1}, {0, 0, 1}
	}));

	auto tangent = createPtr<VBO>(3, StaticMesh::Attributes::TANGENT);
	tangent->setData(std::vector<Vec3>({
		{1, 0, 0}, {1, 0, 0}, {1, 0, 0},
		{1, 0, 0}, {1, 0, 0}, {1, 0, 0}
	}));

	mesh->addVBO(vertex);
	mesh->addVBO(uv);
	mesh->addVBO(normal);
	mesh->addVBO(tangent);

	mesh->prepare();

	return mesh;
}

StaticMesh *StaticMeshPrimitives::cube(float scale) {
	StaticMesh *mesh = new StaticMesh{};

	auto vertex = createPtr<VBO>(3, StaticMesh::Attributes::POSITION);

	std::vector<Vec3> vertices{
		{-1, 1, -1}, {1, 1, 1}, {1, 1, -1},
		{1, 1, 1}, {-1, -1, 1}, {1, -1, 1},
		{-1, 1, 1}, {-1, -1, -1}, {-1, -1, 1},
		{1, -1, -1}, {-1, -1, 1}, {-1, -1, -1},
		{1, 1, -1}, {1, -1, 1}, {1, -1, -1},
		{-1, 1, -1}, {1, -1, -1}, {-1, -1, -1},
		{-1, 1, -1}, {-1, 1, 1}, {1, 1, 1},
		{1, 1, 1}, {-1, 1, 1}, {-1, -1, 1},
		{-1, 1, 1}, {-1, 1, -1}, {-1, -1, -1},
		{1, -1, -1}, {1, -1, 1}, {-1, -1, 1},
		{1, 1, -1}, {1, 1, 1}, {1, -1, 1},
		{-1, 1, -1}, {1, 1, -1}, {1, -1, -1}
	};

	for(auto &vert : vertices) vert *= scale;

	vertex->setData(vertices);

	auto uv = createPtr<VBO>(2, StaticMesh::Attributes::UV);
	uv->setData(std::vector<Vec2>({
		{0.875, 0.5}, {0.625, 0.75}, {0.625, 0.5},
		{0.625, 0.75}, {0.375, 1}, {0.375, 0.75},
		{0.625, 0}, {0.375, 0.25}, {0.375, 0},
		{0.375, 0.5}, {0.125, 0.75}, {0.125, 0.5},
		{0.625, 0.5}, {0.375, 0.75}, {0.375, 0.5},
		{0.625, 0.25}, {0.375, 0.5}, {0.375, 0.25},
		{0.875, 0.5}, {0.875, 0.75}, {0.625, 0.75},
		{0.625, 0.75}, {0.625, 1}, {0.375, 1},
		{0.625, 0}, {0.625, 0.25}, {0.375, 0.25},
		{0.375, 0.5}, {0.375, 0.75}, {0.125, 0.75},
		{0.625, 0.5}, {0.625, 0.75}, {0.375, 0.75},
		{0.625, 0.25}, {0.625, 0.5}, {0.375, 0.5}
	}));

	auto normal = createPtr<VBO>(3, StaticMesh::Attributes::NORMAL);
	normal->setData(std::vector<Vec3>({
		{0, 1, 0}, {0, 1, 0}, {0, 1, 0},
		{0, 0, 1}, {0, 0, 1}, {0, 0, 1},
		{-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
		{0, -1, 0}, {0, -1, 0}, {0, -1, 0},
		{1, 0, 0}, {1, 0, 0}, {1, 0, 0},
		{0, 0, -1}, {0, 0, -1}, {0, 0, -1},
		{0, 1, 0}, {0, 1, 0}, {0, 1, 0},
		{0, 0, 1}, {0, 0, 1}, {0, 0, 1},
		{-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
		{0, -1, 0}, {0, -1, 0}, {0, -1, 0},
		{1, 0, 0}, {1, 0, 0}, {1, 0, 0},
		{0, 0, -1}, {0, 0, -1}, {0, 0, -1}
	}));

	auto tangent = createPtr<VBO>(3, StaticMesh::Attributes::TANGENT);
	tangent->setData(std::vector<Vec3>({
		{-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
		{-0, 1, 0}, {0, 1, 0}, {0, 1, 0},
		{0, 1, -0}, {0, 1, 0}, {0, 1, 0},
		{1, 0, 0}, {1, 0, 0}, {1, 0, 0},
		{0, 1, 0}, {0, 1, 0}, {0, 1, 0},
		{0, 1, 0}, {0, 1, 0}, {0, 1, 0},
		{-1, 0, 0}, {-1, -0, 0}, {-1, 0, 0},
		{0, 1, 0}, {-0, 1, -0}, {0, 1, 0},
		{0, 1, 0}, {0, 1, -0}, {0, 1, 0},
		{1, 0, 0}, {1, 0, 0}, {1, 0, 0},
		{0, 1, 0}, {-0, 1, 0}, {0, 1, 0},
		{0, 1, 0}, {0, 1, 0}, {0, 1, 0}
	}));

	mesh->addVBO(vertex);
	mesh->addVBO(uv);
	mesh->addVBO(normal);
	mesh->addVBO(tangent);

	mesh->prepare();

	return mesh;
}

StaticMesh *StaticMeshPrimitives::invertedCube() {
	StaticMesh *mesh = new StaticMesh{};

	auto vertex = createPtr<VBO>(3, StaticMesh::Attributes::POSITION);
	vertex->setData(std::vector<Vec3>({
		{1, 1, 1}, {-1, 1, -1}, {1, 1, -1},
		{-1, -1, 1}, {1, 1, 1}, {1, -1, 1},
		{-1, -1, -1}, {-1, 1, 1}, {-1, -1, 1},
		{-1, -1, 1}, {1, -1, -1}, {-1, -1, -1},
		{1, -1, 1}, {1, 1, -1}, {1, -1, -1},
		{1, -1, -1}, {-1, 1, -1}, {-1, -1, -1},
		{1, 1, 1}, {-1, 1, 1}, {-1, 1, -1},
		{-1, -1, 1}, {-1, 1, 1}, {1, 1, 1},
		{-1, -1, -1}, {-1, 1, -1}, {-1, 1, 1},
		{-1, -1, 1}, {1, -1, 1}, {1, -1, -1},
		{1, -1, 1}, {1, 1, 1}, {1, 1, -1},
		{1, -1, -1}, {1, 1, -1}, {-1, 1, -1}
	}));

	auto uv = createPtr<VBO>(2, StaticMesh::Attributes::UV);
	uv->setData(std::vector<Vec2>({
		{0.625, 0.75}, {0.875, 0.5}, {0.625, 0.5},
		{0.375, 1}, {0.625, 0.75}, {0.375, 0.75},
		{0.375, 0.25}, {0.625, 0}, {0.375, 0},
		{0.125, 0.75}, {0.375, 0.5}, {0.125, 0.5},
		{0.375, 0.75}, {0.625, 0.5}, {0.375, 0.5},
		{0.375, 0.5}, {0.625, 0.25}, {0.375, 0.25},
		{0.625, 0.75}, {0.875, 0.75}, {0.875, 0.5},
		{0.375, 1}, {0.625, 1}, {0.625, 0.75},
		{0.375, 0.25}, {0.625, 0.25}, {0.625, 0},
		{0.125, 0.75}, {0.375, 0.75}, {0.375, 0.5},
		{0.375, 0.75}, {0.625, 0.75}, {0.625, 0.5},
		{0.375, 0.5}, {0.625, 0.5}, {0.625, 0.25}
	}));

	auto normal = createPtr<VBO>(3, StaticMesh::Attributes::NORMAL);
	normal->setData(std::vector<Vec3>({
		{0, -1, 0}, {0, -1, 0}, {0, -1, 0},
		{0, 0, -1}, {0, 0, -1}, {0, 0, -1},
		{1, 0, 0}, {1, 0, 0}, {1, 0, 0},
		{0, 1, 0}, {0, 1, 0}, {0, 1, 0},
		{-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
		{0, 0, 1}, {0, 0, 1}, {0, 0, 1},
		{0, -1, 0}, {0, -1, 0}, {0, -1, 0},
		{0, 0, -1}, {0, 0, -1}, {0, 0, -1},
		{1, 0, 0}, {1, 0, 0}, {1, 0, 0},
		{0, 1, 0}, {0, 1, 0}, {0, 1, 0},
		{-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
		{0, 0, 1}, {0, 0, 1}, {0, 0, 1}
	}));

	auto tangent = createPtr<VBO>(3, StaticMesh::Attributes::TANGENT);
	tangent->setData(std::vector<Vec3>({
		{-1, 0, -0}, {-1, 0, -0}, {-1, 0, -0},
		{-0, 1, 0}, {-0, 1, 0}, {0, 1, 0},
		{-0, 1, -0}, {-0, 1, -0}, {-0, 1, 0},
		{1, -0, -0}, {1, -0, -0}, {1, -0, -0},
		{0, 1, -0}, {0, 1, -0}, {0, 1, -0},
		{-0, 1, -0}, {-0, 1, -0}, {-0, 1, -0},
		{-1, 0, 0}, {-1, 0, -0}, {-1, 0, -0},
		{0, 1, 0}, {-0, 1, 0}, {-0, 1, 0},
		{0, 1, 0}, {-0, 1, -0}, {-0, 1, -0},
		{1, 0, -0}, {1, -0, -0}, {1, -0, -0},
		{0, 1, -0}, {0, 1, -0}, {0, 1, -0},
		{-0, 1, 0}, {-0, 1, -0}, {-0, 1, -0}
	}));

	mesh->addVBO(vertex);
	mesh->addVBO(uv);
	mesh->addVBO(normal);
	mesh->addVBO(tangent);

	mesh->prepare();

	return mesh;
}
