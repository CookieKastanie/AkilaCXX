#pragma once

#include "akila/common/type_infos.hpp"

namespace akila {
	class Layer {
	public:
		enum class Depth: char {
			BACK = -1,
			MIDDLE = 0,
			FRONT = 1
		};

		Layer();
		virtual ~Layer() = default;

		virtual void update();
		virtual void draw();
		virtual void drawImGui();

	private:
		friend class Layers;

		TypeId typeId;
		Depth depth;
	};
}
