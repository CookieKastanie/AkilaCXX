#include "Akila/core/ResourcePool.hpp"
#include "Akila/files/FileSystem.hpp"
#include "Akila/files/Loader.hpp"
#include "Akila/graphics/gl/Texture.hpp"
#include "Akila/graphics/ShaderBuilder.hpp"
#include "Akila/files/ResourceFileParser.hpp"

using namespace Akila;

ResourcePoolV2::ResourcePoolV2(const std::shared_ptr<Renderer> &renderer) {

}

ResourceReference<Shader> ResourcePoolV2::getShader(const std::string &name) {
	return shaders[name].createReference();
}

ResourceReference<Texture> ResourcePoolV2::getTexture(const std::string &name) {
	return textures[name].createReference();
}

ResourceReference<CubeMapTexture> ResourcePoolV2::getCubeMapTexture(const std::string &name) {
	return cubeMapTextures[name].createReference();
}

ResourceReference<Material> ResourcePoolV2::getMaterial(const std::string &name) {
	return materials[name].createReference();
}

ResourceReference<Mesh> ResourcePoolV2::getMesh(const std::string &name) {
	return meshs[name].createReference();
}

void ResourcePoolV2::setShader(const std::string &name, const Shader *shader) {
	shaders[name].setResource(shader);
}

void ResourcePoolV2::setTexture(const std::string &name, const Texture *texture) {
	textures[name].setResource(texture);
}

void ResourcePoolV2::setCubeMapTexture(const std::string &name, const CubeMapTexture *cubeMapTexture) {
	cubeMapTextures[name].setResource(cubeMapTexture);
}

void ResourcePoolV2::setMaterial(const std::string &name, const Material *material) {
	materials[name].setResource(material);
}

void ResourcePoolV2::setMesh(const std::string &name, const Mesh *mesh) {
	meshs[name].setResource(mesh);
}










///////////////////////////////////////////////////////////////////////////////////////


ResourcePool::ResourcePool(const std::shared_ptr<Renderer> &renderer): renderer{renderer} {
	defaultShader = std::make_shared<Shader>("void main(){gl_Position=vec4(0.);}", "void main(){gl_FragColor=vec4(1.);}");

	defaultTexture = std::make_shared<Texture>();

	defaultCubeMapTexture = std::make_shared<CubeMapTexture>();

	defaultMaterial = std::make_shared<Material>();
	defaultMaterial->setShader(defaultShader);

	defaultMesh = std::make_shared<Mesh>();
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

		defaultMesh->addVBO(vertex);
		defaultMesh->addVBO(uv);
	}

	defaultMesh->prepare();

	setMesh("akila_triangle", defaultMesh);
}

std::shared_ptr<Shader> &ResourcePool::getShader(const std::string &name) {
	auto &&val = shaders[name];
	if(val != nullptr) return val;

	std::cerr << "Shader '" << name << "' does not exist right now" << std::endl;
	return defaultShader;
}

std::shared_ptr<Texture> &ResourcePool::getTexture(const std::string &name) {
	auto &&val = textures[name];
	if(val != nullptr) return val;

	std::cerr << "Texture '" << name << "' does not exist right now" << std::endl;
	return defaultTexture;
}

std::shared_ptr<CubeMapTexture> &ResourcePool::getCubeMapTexture(const std::string &name) {
	auto &&val = cubeMapTextures[name];
	if(val != nullptr) return val;

	std::cerr << "CubeMap '" << name << "' does not exist right now" << std::endl;
	return defaultCubeMapTexture;
}

std::shared_ptr<Material> &ResourcePool::getMaterial(const std::string &name) {
	auto &&val = materials[name];
	if(val != nullptr) return val;
	
	std::cerr << "Material '" << name << "' does not exist right now" << std::endl;
	return defaultMaterial;
}

std::shared_ptr<Mesh> &ResourcePool::getMesh(const std::string &name) {
	auto &&val = meshs[name];
	if(val != nullptr) return val;

	std::cerr << "Mesh '" << name << "' does not exist right now" << std::endl;
	return defaultMesh;
}

void ResourcePool::setShader(const std::string &name, const std::shared_ptr<Shader> &shader) {
	shaders[name] = shader;
}

void ResourcePool::setTexture(const std::string &name, const std::shared_ptr<Texture> &texture) {
	textures[name] = texture;
}

void ResourcePool::setCubeMapTexture(const std::string &name, const std::shared_ptr<CubeMapTexture> &cubeMapTexture) {
	cubeMapTextures[name] = cubeMapTexture;
}

void ResourcePool::setMaterial(const std::string &name, const std::shared_ptr<Material> &material) {
	materials[name] = material;
}

void ResourcePool::setMesh(const std::string &name, const std::shared_ptr<Mesh> &mesh) {
	meshs[name] = mesh;
}

void ResourcePool::loadResourceFile(const std::string &path, const std::function<void()> &cb, TaskManager *taskManger) {
	ResourceFileParser rfp{this, cb, taskManger};
	rfp.fromFile(path);
}
