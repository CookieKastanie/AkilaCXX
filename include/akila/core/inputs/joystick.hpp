#pragma once

#include "akila/core/inputs/controller.hpp"
#include <array>

namespace akila {
	class Joystick: public Controller {
	public:
		bool getButton(unsigned int index) const override { return buttons[index]; }
		float getAxis(unsigned int index) const override { return axes[index]; }
		HatPosition getHat(unsigned int index) const override { return hats[index]; }

		int getButtonCount() const override { return buttonCount; }
		int getAxisCount() const override { return axisCount; }
		int getHatCount() const override { return hatCount; }

		void setDeadZone(unsigned int index, float deadZone) override;

		bool isGamepad() const { return joystickIsGamepad; }

	private:
		friend class Inputs;

		bool joystickIsGamepad;

		int buttonCount = 0;
		int axisCount = 0;
		int hatCount = 0;

		std::array<bool, 128> buttons;
		std::array<float, 128> axes;
		std::array<float, 128> axisDeadZones;
		std::array<HatPosition, 128> hats;

		Joystick(int jid);
		void reset() override;
		void update() override;
	};
}
