#pragma once

#include <functional>
#include "glm/vec2.hpp"
#include "GLFW/glfw3.h"

namespace Akila {
	class Mouse {
	private:
		friend class Display;

		static int const KEY_COUNT = 8;
		bool keysState[KEY_COUNT];
		glm::vec2 position;
		std::function<void(Mouse *)> pressCallback;

	public:
		enum Key: int {
			LEFT = GLFW_MOUSE_BUTTON_LEFT,
			RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
			MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE
		};

		Mouse();
		glm::vec2 &getPosition();
		bool isPressed(Key k) const;

		void onKeyPress(const std::function<void(Mouse *)> &cb);
		void detachPressEvent();

	private:
		void setPosition(float x, float y);
		void setKeyState(Key k, bool state);
		void firePressEvent();
	};
}
