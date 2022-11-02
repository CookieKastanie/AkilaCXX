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

		static std::vector<std::unique_ptr<Layer>> const &listing();

	private:
		friend class Core;

		static std::vector<std::unique_ptr<Layer>> layers;
		static std::vector<Layer*> mounts;
		static std::vector<TypeId> unmounts;

		static void init();
		static void terminate();
		static void updateMounts();
		static void updateUnmounts();
		static void tick();
		static void frame();
		static void gui();
		static void removeAll();
	};
}
