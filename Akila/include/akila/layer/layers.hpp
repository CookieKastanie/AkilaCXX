#pragma once

#include "akila/layer/layer.hpp"
#include <vector>
#include <memory>

namespace akila {
	class Layers {
	public:
		template<typename T, typename ...Args>
		static void add(Layer::Depth depth = Layer::Depth::MIDDLE, Args&& ...args) {
			static_assert(std::is_base_of<Layer, T>::value, "T must derive from akila::Layer");

			remove<T>();

			//insert dans le vecteur selon depth
			Layer *layer = new T{args...};
			layer->depth = depth;
			layer->typeId = getTypeId<T>();

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

	private:
		friend class Core;

		static std::vector<std::unique_ptr<Layer>> layers;

		static inline void Layers::tick() {
			for(auto const &layer : layers) layer->tick();
		}

		static inline void Layers::frame() {
			for(auto const &layer : layers) layer->frame();
		}

		static inline void Layers::gui() {
			for(auto const &layer : layers) layer->gui();
		}

		static void removeAll() {
			layers.clear();
		}
	};
}
