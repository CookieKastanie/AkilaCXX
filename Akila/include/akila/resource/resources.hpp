#pragma once

#include <memory>
#include "akila/memory/ref.hpp"
#include <unordered_map>
#include "akila/resource/resource_map.hpp"
#include "akila/common/type_infos.hpp"

namespace akila {
	class Resources {
	public:
		template<typename T, typename ... Args>
		static Ref<T> create(std::string const &name, Args&& ... args) {
			TypeId id = getTypeId<T>();

			// ajout auto d'un nouveau type
			if(maps.find(id) == maps.end()) registerType<T>();

			ResourceMap<T> *map = static_cast<ResourceMap<T>*>(maps.at(id).get());
			map->set(name, new T{args...});
			return map->get(name);
		}

		template<typename T>
		static Ref<T> get(std::string const &name) {
			TypeId id = getTypeId<T>();
			ResourceMap<T> *map = static_cast<ResourceMap<T>*>(maps.at(id).get());
			return map->get(name);
		}

	private:
		static std::unordered_map<TypeId, std::unique_ptr<IResourceMap>> maps;

		template<typename T>
		static void registerType() {
			TypeId id = getTypeId<T>();
			maps[id] = std::unique_ptr<IResourceMap>(new ResourceMap<T>{});
		}
	};
}
