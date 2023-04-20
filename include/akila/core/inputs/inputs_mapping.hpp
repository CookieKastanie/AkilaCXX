#pragma once

#include <string>
#include <unordered_map>

#include "controller.hpp"

namespace akila {
	class InputBindings {
	public:
		std::unordered_map<unsigned int, std::string> &getButtons() { return buttonBindings; }
		std::unordered_map<unsigned int, std::string> &getAxes() { return axisBindings; }
		std::unordered_map<unsigned int, std::string> &getHats() { return hatBindings; }

	private:
		std::unordered_map<unsigned int, std::string> buttonBindings;
		std::unordered_map<unsigned int, std::string> axisBindings;
		std::unordered_map<unsigned int, std::string> hatBindings;
	};

	class InputsMapping {
	public:
		//InputsMapping() {}

		float getAxis(std::string const &action) { return axes[action]; }
		bool getButton(std::string const &action) { return buttons[action]; }
		Controller::HatPosition getHat(std::string const &action) { return hats[action]; }

		/*/
		void bindKeyboard(std::string_view action, unsigned int key);
		void bindMouseButton(std::string_view action, unsigned int key);


		void bindMouseVelocityXToAxis(std::string_view action);
		void bindMouseVelocityYToAxis(std::string_view action);

		void bindGamepadButton(std::string_view action, unsigned int key, unsigned int gamepadIndex = 0);
		void bindGamepadAxis(std::string_view action, unsigned int key, unsigned int gamepadIndex = 0);

		void bindJoystickButton(std::string_view action, unsigned int index, std::string_view joystickGuid = "");
		void bindJoystickAxis(std::string_view action, unsigned int index, std::string_view joystickGuid = "");
		void bindJoystickHatToButton(std::string_view action, unsigned int index, std::string_view joystickGuid = "");
		void bindJoystickHatToAxis(std::string_view action, unsigned int index, std::string_view joystickGuid = "");
		//*/

	private:
		friend class Inputs;

		std::unordered_map<std::string, InputBindings> bindings;

		std::unordered_map<std::string, float> axes;
		std::unordered_map<std::string, bool> buttons;
		std::unordered_map<std::string, Controller::HatPosition> hats;

		void update();
	};
}
