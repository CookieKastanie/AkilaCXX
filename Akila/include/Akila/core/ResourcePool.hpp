#pragma once

#include <vector>
#include <map>
#include "Akila/graphics/gl/Shader.hpp"
#include "Akila/graphics/gl/Texture.hpp"
#include "Akila/graphics/Material.hpp"
#include "Akila/graphics/Mesh.hpp"
#include "Akila/core/Memory.hpp"

#include "nlohmann/json.hpp"

namespace Akila {
	template<class T>
	class ResourceMap {
	private:
		typedef T*(*G)();
		G generator;
		std::map<std::string, RefAnchor<T>> map;

	public:
		ResourceMap(): generator{[]() -> T* {return nullptr;}} {};

		void setGenerator(G const &generator) {
			this->generator = generator;
		};

		void set(std::string const &name, T *value) {
			auto it = map.find(name);
			if(it == map.end()) map[name].setValue(value);
			else it->second.setValue(value);
		}

		Ref<T> get(std::string const &name) {
			auto it = map.find(name);
			if(it == map.end()) set(name, generator());
			else return it->second.createReference();

			return get(name);
		}

		bool remove(std::string const &name, bool force = false) {
			auto it = map.find(name);

			if(it == map.end()) return false;
			if(it->second.haveReferences() && !force) return false;

			map.erase(it);

			return true;
		}

		void clear(bool force = false) {
			for(auto it = map.begin(); it != map.end();) {
				if(!it->second.haveReferences() || force) it = map.erase(it);
				else it++;
			}
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
		void load(nlohmann::json &file, std::function<void()> const &callback = []() -> void {});
		void load(std::string const &path, std::function<void()> const &callback = []() -> void {});
		void clearAll(bool force = false);
	};
}
