#pragma once

#include <vector>
#include <memory>
#include <map>
#include "Akila/graphics/gl/Shader.hpp"
#include "Akila/graphics/gl/Texture.hpp"
#include "Akila/graphics/Material.hpp"
#include "Akila/graphics/Mesh.hpp"
#include "Akila/graphics/Renderer.hpp"
#include "Akila/core/Task.hpp"

#include "Akila/core/ResourceReference.hpp"

namespace Akila {
	class ResourcePoolV2 {
	private:
		std::shared_ptr<Renderer> renderer;

		ResourceAnchor<Shader> defaultShader;
		ResourceAnchor<Texture> defaultTexture;
		ResourceAnchor<CubeMapTexture> defaultCubeMapTexture;
		ResourceAnchor<Material> defaultMaterial;
		ResourceAnchor<Mesh> defaultMesh;

		std::map<std::string, ResourceAnchor<Shader>> shaders;
		std::map<std::string, ResourceAnchor<Texture>> textures;
		std::map<std::string, ResourceAnchor<CubeMapTexture>> cubeMapTextures;
		std::map<std::string, ResourceAnchor<Material>> materials;
		std::map<std::string, ResourceAnchor<Mesh>> meshs;

	public:
		ResourcePoolV2(const std::shared_ptr<Renderer> &renderer);

		ResourceReference<Shader> getShader(const std::string &name);
		ResourceReference<Texture> getTexture(const std::string &name);
		ResourceReference<CubeMapTexture> getCubeMapTexture(const std::string &name);
		ResourceReference<Material> getMaterial(const std::string &name);
		ResourceReference<Mesh> getMesh(const std::string &name);

		void setShader(const std::string &name, Shader *shader);
		void setTexture(const std::string &name, Texture *texture);
		void setCubeMapTexture(const std::string &name, CubeMapTexture *cubeMapTexture);
		void setMaterial(const std::string &name, Material *material);
		void setMesh(const std::string &name, Mesh *mesh);
	};

	///////////////////////////////////////////////////////////////////////////////////


	class ResourcePool {
	private:
		friend class ResourceFileParser;

		std::shared_ptr<Renderer> renderer;

		std::shared_ptr<Shader> defaultShader;
		std::map<std::string, std::shared_ptr<Shader>> shaders;

		std::shared_ptr<Texture> defaultTexture;
		std::map<std::string, std::shared_ptr<Texture>> textures;

		std::shared_ptr<CubeMapTexture> defaultCubeMapTexture;
		std::map<std::string, std::shared_ptr<CubeMapTexture>> cubeMapTextures;

		std::shared_ptr<Material> defaultMaterial;
		std::map<std::string, std::shared_ptr<Material>> materials;

		std::shared_ptr<Mesh> defaultMesh;
		std::map<std::string, std::shared_ptr<Mesh>> meshs;

	public:
		ResourcePool(const std::shared_ptr<Renderer> &renderer);

		std::shared_ptr<Shader> &getShader(const std::string &name);
		std::shared_ptr<Texture> &getTexture(const std::string &name);
		std::shared_ptr<CubeMapTexture> &getCubeMapTexture(const std::string &name);
		std::shared_ptr<Material> &getMaterial(const std::string &name);
		std::shared_ptr<Mesh> &getMesh(const std::string &name);

		void setShader(const std::string &name, const std::shared_ptr<Shader> &shader);
		void setTexture(const std::string &name, const std::shared_ptr<Texture> &texture);
		void setCubeMapTexture(const std::string &name, const std::shared_ptr<CubeMapTexture> &cubeMapTexture);
		void setMaterial(const std::string &name, const std::shared_ptr<Material> &material);
		void setMesh(const std::string &name, const std::shared_ptr<Mesh> &mesh);

		void loadResourceFile(const std::string &path, const std::function<void()> &cb = []() {}, TaskManager *taskManger = nullptr);
	};
}
