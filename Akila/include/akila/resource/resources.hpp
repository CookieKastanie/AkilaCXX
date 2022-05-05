#pragma once

#include <memory>
#include "akila/memory/ref.hpp"
#include <unordered_map>
#include "akila/resource/resourcemap.hpp"

namespace akila {
	class Resources {
	private:
		using TypeId = std::size_t;

		static std::unordered_map<TypeId, std::unique_ptr<IResourceMap>> maps;

	public:
		template<typename T>
		static void registerType() {
			TypeId id = typeid(T).hash_code();
			
			auto &it = maps.find(id);
			if(it != maps.end()) return;

			maps[id] = std::unique_ptr<IResourceMap>(new ResourceMap<T>{});
		}

		template<typename T, typename ... Args>
		static Ref<T> create(std::string const &name, Args&& ... args) {
			ResourceMap<T> *map = static_cast<ResourceMap<T>*>(maps.at(typeid(T).hash_code()).get());
			map->set(name, new T{std::forward<Args>(args)});
			return map->get(name);
		}

		template<typename T>
		static Ref<T> get(std::string const &name) {
			ResourceMap<T> *map = static_cast<ResourceMap<T>*>(maps.at(typeid(T).hash_code()).get());
			return map->get(name);
		}
	};
}
