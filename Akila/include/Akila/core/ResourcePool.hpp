#pragma once

#include <vector>
#include <memory>
#include <map>
#include "Akila/graphics/gl/Shader.hpp"
#include "Akila/graphics/gl/Texture.hpp"
#include "Akila/graphics/Material.hpp"
#include "Akila/graphics/Mesh.hpp"

#include "Akila/core/ResourceReference.hpp"

namespace Akila {
	template<class T>
	class ResourceMap {
	private:
		ResourceAnchor<T> defaultValue;
		std::map<std::string, ResourceAnchor<T>> map;

	public:
		ResourceMap() {};

		void setDefault(T *value) {
			defaultValue.setResource(value);
		};

		void set(std::string const &name, T *value) {
			auto it = map.find(name);
			if(it == map.end()) map[name].setResource(value);
			else it->second.setResource(value);
		}

		ResourceReference<T> get(std::string const &name) {
			auto it = map.find(name);
			if(it == map.end()) return defaultValue.createReference();
			else return it->second.createReference();
		}

		template<class SubT>
		ResourceReference<SubT> get(std::string const &name) {
			auto it = map.find(name);
			if(it == map.end()) return defaultValue.createReference<SubT>();
			else return it->second.createReference<SubT>();
		}

		bool remove(std::string const &name, bool force = false) {
			auto it = map.find(name);

			if(it == map.end()) return false;
			if(it->second->haveReferences() && !force) return false;

			map.erase(it);

			return true;
		}

		void clear(bool force = false) {
			for(auto it : map) remove(it->first, force);
		}
	};

	class ResourcePool {
	public:
		ResourceMap<Shader> shaders;
		ResourceMap<Texture> textures;
		ResourceMap<CubeMapTexture> cubeMaps;
		ResourceMap<Mesh> meshs;
		ResourceMap<Material> materials;

		ResourcePool();
	};
}
