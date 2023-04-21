#include "akila/core/inputs/inputs_mapping.hpp"
#include "akila/core/inputs/inputs.hpp"

using namespace akila;

void InputsMapping::setBindings(Controller::Guid const &guid, InputBindings const &bdngs) {
	bindings[guid] = bdngs;
}

void InputsMapping::clearBindings(Controller::Guid const &guid) {
	bindings.erase(guid);
}

void InputsMapping::clearBindings() {
	bindings.clear();
}

void InputsMapping::update() {
	std::vector<Controller *> const &controllers = Inputs::getAllControllers();

	for(Controller const *controller : controllers) {
		Controller::Guid const &guid = controller->getGuid();
		auto &binds = bindings[guid];

		for(auto &[index, action] : binds.getButtons()) {
			buttons[action] = controller->getButton(index);
		}

		for(auto &[index, action] : binds.getAxes()) {
			axes[action] = controller->getAxis(index);
		}

		for(auto &[index, action] : binds.getHats()) {
			hats[action] = controller->getHat(index);
		}
	}
}
