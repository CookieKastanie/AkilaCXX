#pragma once

#include "akila/core/layer/layer.hpp"

namespace akila {
	class GameLayer: public Layer {
	public:
		GameLayer();

		void tick() final;
		void frame() final;
		void gui() final;

	private:
		
	};
}
