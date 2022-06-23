#pragma once

#include "akila/memory/ref.hpp"
#include "akila/renderer/shader.hpp"
#include <list>

namespace akila {
	class MaterialInstance {
	public:
	private:
		friend class Material;

		Material *material;

		MaterialInstance(Material *material);
	};

	class Material {
	public:
		Material(std::string const &shader);
		//void set(std::string uniformName);
		void send();
		Ref<MaterialInstance> createInstance();

	private:
		//unsigned int id;
		Shader shader;
		std::list<RefAnchor<MaterialInstance>> instances;

		void eraseEmptyInstances();
	};
}
