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
		float getAxis(std::string const &action) { return axes[action]; }
		bool getButton(std::string const &action) { return buttons[action]; }
		Controller::HatPosition getHat(std::string const &action) { return hats[action]; }

		void setBindings(Controller::Guid const &guid, InputBindings const &bindings);
		void clearBindings(Controller::Guid const &guid);
		void clearBindings();

	private:
		friend class Inputs;

		InputsMapping() {}

		std::unordered_map<Controller::Guid, InputBindings> bindings;

		std::unordered_map<std::string, float> axes;
		std::unordered_map<std::string, bool> buttons;
		std::unordered_map<std::string, Controller::HatPosition> hats;

		void update();
	};
}
