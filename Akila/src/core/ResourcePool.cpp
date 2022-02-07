#include "Akila/core/ResourcePool.hpp"
#include "Akila/graphics/ShaderBuilder.hpp"

using namespace Akila;

ResourcePool::ResourcePool() {
	shaders.setDefault(new Shader{"void main(){gl_Position=vec4(0.);}", "void main(){gl_FragColor=vec4(1.);}"});
	textures.setDefault(new Texture{});
	cubeMaps.setDefault(new CubeMapTexture{});
	materials.setDefault(new Material{});
	//defaultMaterial.createReference()->setShader(defaultShader.createReference());
	meshs.setDefault(new Mesh{});

	auto meshRef = meshs.get("");
	{
		auto vertex = std::make_shared<Akila::VBO>(2, ShaderBuilder::Attributes::A_POSITION);
		vertex->setData(std::vector<glm::vec2>({
			{-1, -1}, {1, -1}, {1, 1},
			{-1, -1}, {1, 1}, {-1, 1}
		}));

		auto uv = std::make_shared<Akila::VBO>(2, ShaderBuilder::Attributes::A_UV);
		uv->setData(std::vector<glm::vec2>({
			{0, 0}, {1, 0}, {1, 1},
			{0, 0}, {1, 1}, {0, 1}
		}));

		meshRef->addVBO(vertex);
		meshRef->addVBO(uv);
	}

	meshRef->prepare();

	//setMesh("akila_triangle", defaultMesh);
}
