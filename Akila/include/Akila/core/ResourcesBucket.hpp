#pragma once

#include <vector>
#include <memory>
#include <map>
#include "Akila/graphics/gl/Shader.hpp"
#include "Akila/graphics/gl/Texture.hpp"
#include "Akila/graphics/Material.hpp"
#include "Akila/graphics/Renderer.hpp"
#include "Akila/core/Task.hpp"

namespace Akila {
	class ResourcesBucket {
	private:
		std::shared_ptr<Renderer> renderer;

		std::shared_ptr<Shader> defaultShader;
		std::map<std::string, std::shared_ptr<Shader>> shaders;

		std::shared_ptr<Texture> defaultTexture;
		std::map<std::string, std::shared_ptr<Texture>> textures;

		std::shared_ptr<Material> defaultMaterial;
		std::map<std::string, std::shared_ptr<Material>> materials;

	public:
		ResourcesBucket(const std::shared_ptr<Renderer> &renderer);

		std::shared_ptr<Shader> &getShader(const std::string &name);
		std::shared_ptr<Texture> &getTexture(const std::string &name);
		std::shared_ptr<Material> &getMaterial(const std::string &name);

		void loadResourceFile(const std::string &path, TaskManager *taskManger = nullptr);
	};
}
