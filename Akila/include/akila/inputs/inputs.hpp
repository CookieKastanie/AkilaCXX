#pragma once

#include <GLFW/glfw3.h>
#include <array>
#include <string>
#include "akila/math/math.hpp"

namespace akila {
	namespace internal {
		class WindowEvents;
	}

	class Inputs {
	public:
		enum class Key: int {
			A = GLFW_KEY_A,
			B = GLFW_KEY_B,
			C = GLFW_KEY_C,
			D = GLFW_KEY_D,
			E = GLFW_KEY_E,
			F = GLFW_KEY_F,
			G = GLFW_KEY_G,
			H = GLFW_KEY_H,
			I = GLFW_KEY_I,
			J = GLFW_KEY_J,
			K = GLFW_KEY_K,
			L = GLFW_KEY_L,
			M = GLFW_KEY_M,
			N = GLFW_KEY_N,
			O = GLFW_KEY_O,
			P = GLFW_KEY_P,
			Q = GLFW_KEY_Q,
			R = GLFW_KEY_R,
			S = GLFW_KEY_S,
			T = GLFW_KEY_T,
			U = GLFW_KEY_U,
			V = GLFW_KEY_V,
			W = GLFW_KEY_W,
			X = GLFW_KEY_X,
			Y = GLFW_KEY_Y,
			Z = GLFW_KEY_Z,

			ENTER = GLFW_KEY_ENTER,
			ESC = GLFW_KEY_ESCAPE,

			SPACE = GLFW_KEY_SPACE,
			CTRL_LEFT = GLFW_KEY_LEFT_CONTROL,
			SHIFT_LEFT = GLFW_KEY_LEFT_SHIFT,

			TAB = GLFW_KEY_TAB,

			UP = GLFW_KEY_UP,
			DOWN = GLFW_KEY_DOWN,
			RIGHT = GLFW_KEY_RIGHT,
			LEFT = GLFW_KEY_LEFT,

			///////////////////////////////////

			LEFT_CLICK = GLFW_MOUSE_BUTTON_1,
			RIGHT_CLICK = GLFW_MOUSE_BUTTON_2,
			MIDDLE_CLICK = GLFW_MOUSE_BUTTON_3,
		};

		static bool isPressed(Key key);
		//static bool isPressed(std::string const &name); pour du key mapping

		static Vec2 getMousePosition();
		static Vec2 getMouseVelocity();
		static Vec2 getMouseScrollVelocity();

	private:
		friend class internal::WindowEvents;

		static std::array<bool, 512> keysState;
		static Vec2 mousePosition;
		static Vec2 mouseVelocity;
		static Vec2 mouseScrollVelocity;

		static void init();

		static void setInputState(Key key, bool state);
		static void setMousePosition(Vec2 const &pos);
		static void setMouseVelocity(Vec2 const &vel);
		static void setMouseScrollVelocity(Vec2 const &scrollVel);
	};
}
