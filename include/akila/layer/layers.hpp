#pragma once

#include "akila/layer/layer.hpp"
#include <vector>
#include <memory>

namespace akila {
	class Layers {
	public:
		/*/
		template<typename T, typename ...Args>
		static void add(Layer::Depth depth = Layer::Depth::MIDDLE, Args&& ...args) {
			static_assert(std::is_base_of<Layer, T>::value, "T must derive from akila::Layer");

			remove<T>();

			//insert dans le vecteur selon depth
			Layer *layer = new T{args...};
			layer->depth = depth;
			layer->typeId = getTypeId<T>();
			layer->typeName = getTypeName<T>();

			for(auto it = layers.begin(); it != layers.end(); ++it) {
				if(it->get()->depth > depth) {
					layers.emplace(it, layer);
					return;
				}
			}

			layers.emplace_back(layer);
		}

		template<typename T>
		static void remove() {
			TypeId typeId = getTypeId<T>();

			for(auto &it = layers.begin(); it != layers.end(); ++it) {
				if(it->get()->typeId == typeId) {
					layers.erase(it);
					break;
				}
			}
		}
		//*/

		template<typename T, typename ...Args>
		static void add(Layer::Depth depth = Layer::Depth::MIDDLE, Args&& ...args) {
			static_assert(std::is_base_of<Layer, T>::value, "T must derive from akila::Layer");

			remove<T>();
			//updateUnmounts();

			Layer *layer = new T{args...};
			layer->depth = depth;
			layer->typeId = getTypeId<T>();
			layer->typeName = getTypeName<T>();

			mounts.push_back(layer);
		}

		template<typename T>
		static void remove() {
			TypeId typeId = getTypeId<T>();
			unmounts.push_back(typeId);
		}

		static std::vector<std::unique_ptr<Layer>> const &listing() {
			return layers;
		}

	private:
		friend class Core;

		static std::vector<std::unique_ptr<Layer>> layers;
		static std::vector<Layer*> mounts;
		static std::vector<TypeId> unmounts;

		static void init() {

		}

		static void terminate() {
			layers.clear();
			mounts.clear();
			unmounts.clear();
		}

		static void updateMounts() {
			while(!mounts.empty()) {
				Layer *layer = mounts.back();
				mounts.pop_back();

				for(auto it = layers.begin(); it != layers.end(); ++it) {
					if(it->get()->depth > layer->depth) {
						layers.emplace(it, layer);
						layer->onMount();
						return;
					}
				}

				layers.emplace_back(layer);
				layer->onMount();
			}
		}

		static void updateUnmounts() {
			while(!unmounts.empty()) {
				TypeId typeId = unmounts.back();
				unmounts.pop_back();

				for(auto &it = layers.begin(); it != layers.end(); ++it) {
					if(it->get()->typeId == typeId) {
						it->get()->onUnmount();
						layers.erase(it);
						break;
					}
				}
			}
		}

		static void Layers::tick() {
			for(auto const &layer : layers) layer->tick();
		}

		static void Layers::frame() {
			for(auto const &layer : layers) layer->frame();
		}

		static void Layers::gui() {
			for(auto const &layer : layers) layer->gui();
		}

		static void removeAll() {
			layers.clear();
		}
	};
}
