#pragma once

#include "akila/akila.hpp"
#include "akila/engine/layers/debug_timings.hpp"

namespace akila {
	class DebugLayer: public Layer {
	public:
		DebugLayer();
		void gui() override;

	private:
		bool show;
		std::deque<float> fpsQueue;
		int tabIndex;

		Listener keyListener;

		int updateMeanFPS();

		void guiTimings();
		void guiResources();
		void guiGLLogs();
		void guiLayers();
		void guiECS();
		void guiControllers();
	};
}
