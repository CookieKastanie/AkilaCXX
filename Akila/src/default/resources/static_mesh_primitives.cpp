#include "akila/default/resources/static_mesh_primitives.hpp"

using namespace akila;

StaticMesh *SaticMeshPrimitives::screenTriangle() {
	StaticMesh *mesh = new StaticMesh{};

	auto vertex = createPtr<VBO>(2, StaticMesh::Attributes::POSITION);
	vertex->setData(std::vector<glm::vec2>({
		{3, 1}, {-1, 1}, {-1, -3}
	}));

	auto uv = createPtr<VBO>(2, StaticMesh::Attributes::UV);
	uv->setData(std::vector<glm::vec2>({
		{2, 1}, {0, 1}, {0, -1}
	}));

	mesh->addVBO(vertex);
	mesh->addVBO(uv);

	mesh->prepare();

	return mesh;
}

StaticMesh *SaticMeshPrimitives::quad() {
	StaticMesh *mesh = new StaticMesh{};

	auto vertex = createPtr<VBO>(2, StaticMesh::Attributes::POSITION);
	vertex->setData(std::vector<glm::vec2>({
		{-1, -1}, {1, -1}, {1, 1},
		{-1, -1}, {1, 1}, {-1, 1}
	}));

	auto uv = createPtr<VBO>(2, StaticMesh::Attributes::UV);
	uv->setData(std::vector<glm::vec2>({
		{0, 0}, {1, 0}, {1, 1},
		{0, 0}, {1, 1}, {0, 1}
	}));

	auto normal = createPtr<VBO>(3, StaticMesh::Attributes::NORMAL);
	vertex->setData(std::vector<glm::vec3>({
		{0, 0, 1}, {0, 0, 1}, {0, 0, 1},
		{0, 0, 1}, {0, 0, 1}, {0, 0, 1}
	}));

	auto tangent = createPtr<VBO>(3, StaticMesh::Attributes::TANGENT);
	vertex->setData(std::vector<glm::vec3>({
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

StaticMesh *SaticMeshPrimitives::cube() {
	StaticMesh *mesh = new StaticMesh{};

	auto vertex = createPtr<VBO>(3, StaticMesh::Attributes::POSITION);
	vertex->setData(std::vector<Vec3>({
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
	}));

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

StaticMesh *SaticMeshPrimitives::invertedCube() {
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
