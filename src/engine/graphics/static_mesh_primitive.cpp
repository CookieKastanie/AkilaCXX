#include "akila/engine/graphics/static_mesh_primitive.hpp"

using namespace akila;

StaticMesh *StaticMeshPrimitive::screenTriangle() {
	StaticMesh *mesh = new StaticMesh{};

	auto positions = createPtr<VBO>(2, StaticMesh::Attributes::POSITION);
	positions->setData(std::vector<Vec2>({
		{3, 1}, {-1, 1}, {-1, -3}
	}));

	auto texcoords = createPtr<VBO>(2, StaticMesh::Attributes::TEXCOORD_0);
	texcoords->setData(std::vector<Vec2>({
		{2, 1}, {0, 1}, {0, -1}
	}));

	mesh->addVBO(positions);
	mesh->addVBO(texcoords);

	Mesh::Bounds bounds;
	bounds.min = {-1.f, -3.f, 0.f};
	bounds.max = {3.f, 1.f, 0.f};
	bounds.radius = 3.f;
	bounds.squaredRadius = bounds.radius * bounds.radius;
	mesh->setBounds(bounds);

	mesh->prepare();

	return mesh;
}

StaticMesh *StaticMeshPrimitive::quad(float scale) {
	StaticMesh *mesh = new StaticMesh{};

	auto positions = createPtr<VBO>(2, StaticMesh::Attributes::POSITION);
	std::vector<Vec2> positionData{
		{-1, -1}, {1, -1}, {1, 1},
		{-1, -1}, {1, 1}, {-1, 1}
	};

	for(auto &pos : positionData) pos *= scale;
	positions->setData(positionData);

	auto texcoords = createPtr<VBO>(2, StaticMesh::Attributes::TEXCOORD_0);
	texcoords->setData(std::vector<Vec2>({
		{0, 0}, {1, 0}, {1, 1},
		{0, 0}, {1, 1}, {0, 1}
	}));

	auto normals = createPtr<VBO>(3, StaticMesh::Attributes::NORMAL);
	normals->setData(std::vector<Vec3>({
		{0, 0, 1}, {0, 0, 1}, {0, 0, 1},
		{0, 0, 1}, {0, 0, 1}, {0, 0, 1}
	}));

	auto tangents = createPtr<VBO>(3, StaticMesh::Attributes::TANGENT);
	tangents->setData(std::vector<Vec3>({
		{1, 0, 0}, {1, 0, 0}, {1, 0, 0},
		{1, 0, 0}, {1, 0, 0}, {1, 0, 0}
	}));

	mesh->addVBO(positions);
	mesh->addVBO(texcoords);
	mesh->addVBO(normals);
	mesh->addVBO(tangents);

	Mesh::Bounds bounds;
	bounds.min = {-scale, -scale, 0.f};
	bounds.max = {scale, scale, 0.f};
	bounds.radius = sqrt(scale * scale * 2.f) / 2.f;
	bounds.squaredRadius = bounds.radius * bounds.radius;
	mesh->setBounds(bounds);

	mesh->prepare();

	return mesh;
}

StaticMesh *StaticMeshPrimitive::cube(float scale) {
	StaticMesh *mesh = new StaticMesh{};

	auto positions = createPtr<VBO>(3, StaticMesh::Attributes::POSITION);
	std::vector<Vec3> positionData{
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

	for(auto &pos : positionData) pos *= scale;
	positions->setData(positionData);

	auto texcoords = createPtr<VBO>(2, StaticMesh::Attributes::TEXCOORD_0);
	texcoords->setData(std::vector<Vec2>({
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

	auto normals = createPtr<VBO>(3, StaticMesh::Attributes::NORMAL);
	normals->setData(std::vector<Vec3>({
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

	auto tangents = createPtr<VBO>(3, StaticMesh::Attributes::TANGENT);
	tangents->setData(std::vector<Vec3>({
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

	mesh->addVBO(positions);
	mesh->addVBO(texcoords);
	mesh->addVBO(normals);
	mesh->addVBO(tangents);

	Mesh::Bounds bounds;
	bounds.min = {-scale, -scale, -scale};
	bounds.max = {scale, scale, scale};
	bounds.radius = sqrt(scale * scale * 2.f) / 2.f;
	bounds.squaredRadius = bounds.radius * bounds.radius;
	mesh->setBounds(bounds);

	mesh->prepare();

	return mesh;
}

StaticMesh *StaticMeshPrimitive::invertedCube(float scale) {
	StaticMesh *mesh = new StaticMesh{};

	auto positions = createPtr<VBO>(3, StaticMesh::Attributes::POSITION);
	std::vector<Vec3> positionData{
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
	};

	for(auto &pos : positionData) pos *= scale;
	positions->setData(positionData);

	auto texcoords = createPtr<VBO>(2, StaticMesh::Attributes::TEXCOORD_0);
	texcoords->setData(std::vector<Vec2>({
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

	auto normals = createPtr<VBO>(3, StaticMesh::Attributes::NORMAL);
	normals->setData(std::vector<Vec3>({
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

	auto tangents = createPtr<VBO>(3, StaticMesh::Attributes::TANGENT);
	tangents->setData(std::vector<Vec3>({
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

	mesh->addVBO(positions);
	mesh->addVBO(texcoords);
	mesh->addVBO(normals);
	mesh->addVBO(tangents);

	Mesh::Bounds bounds;
	bounds.min = {-scale, -scale, -scale};
	bounds.max = {scale, scale, scale};
	bounds.radius = sqrt(scale * scale * 2.f) / 2.f;
	bounds.squaredRadius = bounds.radius * bounds.radius;
	mesh->setBounds(bounds);

	mesh->prepare();

	return mesh;
}
