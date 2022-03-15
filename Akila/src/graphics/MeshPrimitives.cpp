#include "Akila/graphics/MeshPrimitives.hpp"

#include "Akila/graphics/ShaderBuilder.hpp"

using namespace Akila;

Mesh *MeshPrimitives::screenTriangle() {
	Mesh *mesh = new Mesh{};

	auto vertex = createPtr<VBO>(2, ShaderBuilder::Attributes::A_POSITION);
	vertex->setData(std::vector<glm::vec2>({
		{3, 1}, {-1, 1}, {-1, -3}
	}));

	auto uv = createPtr<VBO>(2, ShaderBuilder::Attributes::A_UV);
	uv->setData(std::vector<glm::vec2>({
		{2, 1}, {0, 1}, {0, -1}
	}));

	mesh->addVBO(vertex);
	mesh->addVBO(uv);

	mesh->prepare();

	return mesh;
}

Mesh *MeshPrimitives::quad() {
	Mesh *mesh = new Mesh{};

	auto vertex = createPtr<VBO>(2, ShaderBuilder::Attributes::A_POSITION);
	vertex->setData(std::vector<glm::vec2>({
		{-1, -1}, {1, -1}, {1, 1},
		{-1, -1}, {1, 1}, {-1, 1}
	}));

	auto uv = createPtr<VBO>(2, ShaderBuilder::Attributes::A_UV);
	uv->setData(std::vector<glm::vec2>({
		{0, 0}, {1, 0}, {1, 1},
		{0, 0}, {1, 1}, {0, 1}
	}));

	auto normal = createPtr<VBO>(3, ShaderBuilder::Attributes::A_NORMAL);
	vertex->setData(std::vector<glm::vec3>({
		{0, 0, 1}, {0, 0, 1}, {0, 0, 1},
		{0, 0, 1}, {0, 0, 1}, {0, 0, 1}
	}));

	auto tangent = createPtr<VBO>(3, ShaderBuilder::Attributes::A_TANGENT);
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

Mesh *MeshPrimitives::cube() {
	Mesh *mesh = new Mesh{};

	auto vertex = createPtr<VBO>(3, ShaderBuilder::Attributes::A_POSITION);
	vertex->setData(std::vector<glm::vec3>({
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

	auto uv = createPtr<VBO>(2, ShaderBuilder::Attributes::A_UV);
	uv->setData(std::vector<glm::vec2>({
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

	auto normal = createPtr<VBO>(3, ShaderBuilder::Attributes::A_NORMAL);
	normal->setData(std::vector<glm::vec3>({
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

	auto tangent = createPtr<VBO>(3, ShaderBuilder::Attributes::A_TANGENT);
	tangent->setData(std::vector<glm::vec3>({
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

Mesh *MeshPrimitives::invertedCube() {
	Mesh *mesh = new Mesh{};

	auto vertex = createPtr<VBO>(3, ShaderBuilder::Attributes::A_POSITION);
	vertex->setData(std::vector<glm::vec3>({
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

	auto uv = createPtr<VBO>(2, ShaderBuilder::Attributes::A_UV);
	uv->setData(std::vector<glm::vec2>({
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

	auto normal = createPtr<VBO>(3, ShaderBuilder::Attributes::A_NORMAL);
	normal->setData(std::vector<glm::vec3>({
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

	auto tangent = createPtr<VBO>(3, ShaderBuilder::Attributes::A_TANGENT);
	tangent->setData(std::vector<glm::vec3>({
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
