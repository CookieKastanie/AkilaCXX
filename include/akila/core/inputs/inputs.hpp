#pragma once

#include "akila/core/window/window.hpp"
#include "akila/core/math/math.hpp"
#include "akila/core/memory/ref.hpp"
#include "akila/core/inputs/keyboard.hpp"
#include "akila/core/inputs/mouse.hpp"
#include "akila/core/inputs/gamepad.hpp"
#include "akila/core/inputs/joystick.hpp"
#include "akila/core/inputs/inputs_mapping.hpp"

#include <array>
#include <unordered_map>
#include <string>

namespace akila {
	namespace internal {
		class WindowEvents;
	}

	class Inputs {
	public:
		struct InitValues {
			bool gamepadAsJoystick = false;
		};

		static Keyboard const &getKeyboard() { return keyboard; }
		static Mouse const &getMouse() { return mouse; }

		static std::unordered_map<int, RefAnchor<Gamepad>> const &getGamepads() { return gamepads; }
		static std::unordered_map<Controller::Guid, RefAnchor<Joystick>> const &getJoysticks() { return joysticks; }
		
		static std::vector<Controller *> const &getAllControllers() { return controllerAlias; }

	private:
		friend class Core;
		friend class internal::WindowEvents;

		static Keyboard keyboard;
		static Mouse mouse;
		static std::unordered_map<int, RefAnchor<Gamepad>> gamepads;
		static std::unordered_map<Controller::Guid, RefAnchor<Joystick>> joysticks;

		static std::vector<Controller *> controllerAlias;

		static bool gamepadAsJoystick;

		static void init(InitValues const &initVals);
		static void terminate();

		static void setKeyboardKey(Keyboard::Key key, bool state) { keyboard.setKey(key, state); }
		static void setMouseButton(Mouse::Button button, bool state) { mouse.setButton(button, state); }

		static void updateControllers();
		static void updateCurrentBindings();

		static void setMousePosition(Vec2 const &pos) { mouse.setPosition(pos); }
		static void setClipSpaceMousePosition(Vec2 const &pos) { mouse.setClipSpacePosition(pos); }
		static void setMouseVelocity(Vec2 const &vel) { mouse.setVelocity(vel); }
		static void setMouseScrollVelocity(Vec2 const &scrollVel) { mouse.setScrollVelocity(scrollVel); }

		// return true if similar controller guid is found
		static bool addController(int id);
		static void removeController(int id);
	};
}
