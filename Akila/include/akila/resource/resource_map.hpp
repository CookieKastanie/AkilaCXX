#pragma once

#include "akila/memory/ref.hpp"
#include <string>
#include <unordered_map>

namespace akila::internal {
	class IResourceMap {
	public:
		IResourceMap() = default;
		virtual ~IResourceMap() = default;
	};

	template<class T>
	class ResourceMap: public IResourceMap {
	public:
		ResourceMap() = default;

		void set(std::string const &name, T *value) {
			auto it = map.find(name);
			if(it == map.end()) map[name].setValue(value);
			else it->second.setValue(value);
		}

		Ref<T> get(std::string const &name) {
			auto it = map.find(name);
			if(it == map.end()) set(name, new T{});
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

	private:
		std::unordered_map<std::string, RefAnchor<T>> map;
	};
}
