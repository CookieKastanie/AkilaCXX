#pragma once

#include "akila/memory/ref.hpp"
#include <string>
#include <unordered_map>

namespace akila::internal {
	class IResourceMap {
	public:
		IResourceMap() = default;
		virtual ~IResourceMap() = default;
		
		std::unordered_map<std::string, IRefAnchor&> const &listing() {
			return mapMirror;
		}

		virtual bool remove(std::string const &name, bool force = false) = 0;
		virtual void clear(bool force = false) = 0;

	protected:
		std::unordered_map<std::string, IRefAnchor&> mapMirror;
	};

	template<class T>
	class ResourceMap: public IResourceMap {
	public:
		ResourceMap() = default;

		void set(std::string const &name, T *value) {
			auto it = map.find(name);
			if(it == map.end()) {
				RefAnchor<T> &ra = map[name];
				ra.setValue(value);
				mapMirror.emplace(name, ra);
			} else {
				it->second.setValue(value);
			}
		}

		Ref<T> get(std::string const &name) {
			auto it = map.find(name);
			if(it == map.end()) {
				set(name, new T{});
			} else {
				return it->second.createReference();
			}

			return get(name);
		}

		bool remove(std::string const &name, bool force = false) override {
			auto it = map.find(name);

			if(it == map.end()) return false;
			if(it->second.haveReferences() && !force) return false;

			mapMirror.erase(mapMirror.find(name));
			map.erase(it);

			return true;
		}

		void clear(bool force = false) override {
			for(auto it = map.begin(); it != map.end();) {
				if(!it->second.haveReferences() || force) {

					for(auto m_it = mapMirror.begin(); m_it != mapMirror.end(); ++m_it) {
						if(&m_it->second == &it->second) {
							mapMirror.erase(m_it);
							break;
						}
					}		

					it = map.erase(it);
				}
				else {
					it++;
				}
			}
		}

	private:
		std::unordered_map<std::string, RefAnchor<T>> map;
	};
}
