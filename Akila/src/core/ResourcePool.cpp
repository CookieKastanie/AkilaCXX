#include "Akila/core/ResourcePool.hpp"
#include "Akila/graphics/ShaderBuilder.hpp"

using namespace Akila;

ResourcePool::ResourcePool() {
	shaders.setGenerator([]() {
		return new Shader{"void main(){gl_Position=vec4(0.);}", "void main(){gl_FragColor=vec4(1.);}"};
	});

	textures.setGenerator([]() {
		return new Texture{};
	});

	cubeMaps.setGenerator([]() {
		return new CubeMapTexture{};
	});

	materials.setGenerator([]() {
		return new Material{};
	});

	meshs.setGenerator([]() {
		return new Mesh{};
	});
}
