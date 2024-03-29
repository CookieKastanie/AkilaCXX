#pragma once

#include <memory>
#include "akila/core/memory/ref.hpp"
#include <unordered_map>
#include "akila/core/resources/resource_map.hpp"
#include "akila/core/common/type_infos.hpp"
#include "akila/core/resources/loader.hpp"
#include "akila/core/resources/loading_instance.hpp"

namespace akila {
	namespace internal {
		class LoadingInstance;
	}

	class Resources {
	public:
		template<typename T, typename ...Args>
		static Ref<T> create(std::string const &name, Args&& ...args) {
			TypeId id = getTypeId<T>();

			// ajout auto d'un nouveau type
			if(maps.find(id) == maps.end()) registerType<T>();

			internal::ResourceMap<T> *map = static_cast<internal::ResourceMap<T>*>(maps.at(id).get());
			map->set(name, new T{args...});
			return map->get(name);
		}

		template<typename T>
		static Ref<T> set(std::string const &name, T *value) {
			TypeId id = getTypeId<T>();

			// ajout auto d'un nouveau type
			if(maps.find(id) == maps.end()) registerType<T>();

			internal::ResourceMap<T> *map = static_cast<internal::ResourceMap<T>*>(maps.at(id).get());
			map->set(name, value);
			return map->get(name);
		}

		template<typename T>
		static Ref<T> get(std::string const &name) {
			TypeId id = getTypeId<T>();

			// ajout auto d'un nouveau type
			auto it = maps.find(id);
			if(it == maps.end()) {
				registerType<T>();
				it = maps.find(id);
			}

			internal::ResourceMap<T> *map = static_cast<internal::ResourceMap<T>*>(it->second.get());
			//internal::ResourceMap<T> *map = static_cast<internal::ResourceMap<T>*>(maps.at(id).get());
			return map->get(name);
		}

		// l'instance doit automatiquement se retirer de la liste une fois les chargements finis
		static void load(std::string const &path, std::function<void()> const &callback) {
			loadingInstances.push_back(internal::LoadingInstance{path, callback});
			loadingInstances.back().start();
		}

		static void load(std::initializer_list<std::string> const &paths, std::function<void()> const &callback) {
			loadingInstances.push_back(internal::LoadingInstance{paths, callback});
			loadingInstances.back().start();
		}

		template<typename T>
		static void registerLoader() {
			Loader *loader = new T();

			for(std::size_t i = 0; i < loaders.size(); ++i) {
				if(loaders[i]->getListName() == loader->getListName()) {
					loaders[i] = std::unique_ptr<Loader>(loader);
					return;
				}
			}

			loaders.push_back(std::unique_ptr<Loader>(loader));
		}

		template<typename T>
		static bool remove(std::string const &name, bool force = false) {
			TypeId id = getTypeId<T>();

			auto it = maps.find(id);
			if(it == maps.end()) return false;

			return it->second->remove(name, force);
		}

		template<typename T>
		static void clean(bool force = false) {
			TypeId id = getTypeId<T>();

			auto it = maps.find(id);
			if(it == maps.end()) return;

			it->second->clear(force);
		}

		static void cleanAll(bool force = false) {
			for(std::size_t i = 0; i < maps.size(); ++i)
			for(auto &m : maps) {
				m.second->clear(force);
			}
		}

		static std::unordered_map<TypeId, std::string> const &listing() {
			return mapNames;
		}

		static std::unordered_map<std::string, IRefAnchor&> const &listing(TypeId typeId) {
			return maps.at(typeId)->listing();
		}

	private:
		friend class Core;

		static std::unordered_map<TypeId, std::unique_ptr<internal::IResourceMap>> maps;
		static std::unordered_map<TypeId, std::string> mapNames;

		friend class internal::LoadingInstance;
		static std::vector<std::unique_ptr<Loader>> loaders;
		static std::vector<internal::LoadingInstance> loadingInstances;

		static void init() {
			
		}

		static void terminate() {
			maps.clear();
			mapNames.clear();
			loaders.clear();
			loadingInstances.clear();
		}

		template<typename T>
		static void registerType() {
			TypeId id = getTypeId<T>();

			if(maps.find(id) != maps.end()) return;

			maps[id] = std::unique_ptr<internal::IResourceMap>(new internal::ResourceMap<T>{});
			mapNames[id] = getTypeName<T>();
		}
	};
}
