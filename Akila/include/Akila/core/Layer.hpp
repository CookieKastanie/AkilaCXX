#pragma once

#include <memory>
#include <vector>
#include <iostream>

namespace Akila {
	class Layer {
	public:
		enum class PreferredDepth: int {
			BACK = -1,
			MIDDLE = 0,
			FRONT = 1
		};

	private:
		friend class LayerManager;
		int index;
		PreferredDepth depth;

	public:
		Layer();
		virtual ~Layer();

		virtual void update();
		virtual void draw();

		//#ifdef IMGUI
		virtual void drawImGui();
		//#endif
		
	};

	class LayerManager {
	private:
		std::vector<std::shared_ptr<Layer>> layers;

	public:
		LayerManager();

		void add(const std::shared_ptr<Layer> &layer,
			Layer::PreferredDepth depth = Layer::PreferredDepth::MIDDLE);
		void add(Layer *layer,
			Layer::PreferredDepth depth = Layer::PreferredDepth::MIDDLE);
		void remove(Layer *layer);
		void update();
		void draw();

		#ifdef IMGUI
		void drawImGui();
		#endif
	};
}
