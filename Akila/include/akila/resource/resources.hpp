#pragma once

#include <memory>
#include "akila/memory/ref.hpp"
#include <unordered_map>
#include "akila/resource/resourcemap.hpp"
#include "akila/common/typename.hpp"

namespace akila {
	class Resources {
	public:
		template<typename T, typename ... Args>
		static Ref<T> create(std::string const &name, Args&& ... args) {
			TypeName typeName = getTypeName<T>();

			// ajout auto d'un nouveau type
			if(maps.find(typeName) == maps.end()) registerType<T>();

			ResourceMap<T> *map = static_cast<ResourceMap<T>*>(maps.at(typeName).get());
			map->set(name, new T{args...});
			return map->get(name);
		}

		template<typename T>
		static Ref<T> get(std::string const &name) {
			TypeName typeName = getTypeName<T>();
			ResourceMap<T> *map = static_cast<ResourceMap<T>*>(maps.at(typeName).get());
			return map->get(name);
		}

	private:
		static std::unordered_map<TypeName, std::unique_ptr<IResourceMap>> maps;

		template<typename T>
		static void registerType() {
			TypeName name = getTypeName<T>();
			maps[name] = std::unique_ptr<IResourceMap>(new ResourceMap<T>{});
		}
	};
}
