#pragma once

#include "akila/core/inputs/controller.hpp"
#include <array>

namespace akila {
	class Gamepad: public Controller {
	public:
		enum class Button: int {
			A = GLFW_GAMEPAD_BUTTON_A,
			B = GLFW_GAMEPAD_BUTTON_B,
			X = GLFW_GAMEPAD_BUTTON_X,
			Y = GLFW_GAMEPAD_BUTTON_Y,

			LEFT_BUMPER = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
			RIGHT_BUMPER = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,

			BACK = GLFW_GAMEPAD_BUTTON_BACK,
			START = GLFW_GAMEPAD_BUTTON_START,
			GUIDE = GLFW_GAMEPAD_BUTTON_GUIDE,

			LEFT_THUMB = GLFW_GAMEPAD_BUTTON_LEFT_THUMB,
			RIGHT_THUMB = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,

			DPAD_UP = GLFW_GAMEPAD_BUTTON_DPAD_UP,
			DPAD_RIGHT = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
			DPAD_DOWN = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
			DPAD_LEFT = GLFW_GAMEPAD_BUTTON_DPAD_LEFT,

			CROSS = GLFW_GAMEPAD_BUTTON_CROSS,
			CIRCLE = GLFW_GAMEPAD_BUTTON_CIRCLE,
			SQUARE = GLFW_GAMEPAD_BUTTON_SQUARE,
			TRIANGLE = GLFW_GAMEPAD_BUTTON_TRIANGLE
		};

		enum class Axis: int {
			LEFT_X = GLFW_GAMEPAD_AXIS_LEFT_X,
			LEFT_Y = GLFW_GAMEPAD_AXIS_LEFT_Y,
			
			RIGHT_X = GLFW_GAMEPAD_AXIS_RIGHT_X,
			RIGHT_Y = GLFW_GAMEPAD_AXIS_RIGHT_Y,

			LEFT_TRIGGER = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,
			RIGHT_TRIGGER = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER
		};

		bool getButton(unsigned int index) const override { return state.buttons[index]; }
		float getAxis(unsigned int index) const override { return state.axes[index]; }
		HatPosition getHat(unsigned int index) const override { (void)index; return HatPosition::CENTERED; }

		bool getButton(Button button) const { return state.buttons[static_cast<int>(button)];}
		float getAxis(Axis axis) const { return state.axes[static_cast<int>(axis)]; }

		int getButtonCount() const override { return GLFW_GAMEPAD_BUTTON_LAST + 1; }
		int getAxisCount() const override { return GLFW_GAMEPAD_AXIS_LAST + 1; }
		int getHatCount() const override { return 0; }

		void setDeadZone(unsigned int index, float deadZone) override;

		int getIndex() const { return index; }

		Controller::Guid getRealGuid() { return realGuid; }

	private:
		friend class Inputs;

		Gamepad(int jid, int index);
		void reset() override;
		void update() override;

		void assignIndex(int index);

		Guid realGuid;
		int index;

		GLFWgamepadstate state;
		std::array<float, GLFW_GAMEPAD_AXIS_LAST + 1> axisDeadZones;
	};
}
