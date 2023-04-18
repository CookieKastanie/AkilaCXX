#pragma once

#include "akila/core/window/window.hpp"
#include <string>

namespace akila {
	namespace internal {
		class WindowEvents;
	}

	class Controller {
	public:
		using Guid = std::string;

		static constexpr char const *KEYBOARD_GUID = "KEYBOARD";
		static constexpr char const *MOUSE_GUID = "MOUSE";
		static constexpr char const *GAMEPAD_GUID_BASE = "GAMEPAD_";

		enum class HatPosition: int {
			CENTERED = GLFW_HAT_CENTERED,
			UP = GLFW_HAT_UP,
			RIGHT = GLFW_HAT_RIGHT,
			DOWN = GLFW_HAT_DOWN,
			LEFT = GLFW_HAT_LEFT,
			RIGHT_UP = GLFW_HAT_RIGHT_UP,
			RIGHT_DOWN = GLFW_HAT_RIGHT_DOWN,
			LEFT_UP = GLFW_HAT_LEFT_UP,
			LEFT_DOWN = GLFW_HAT_LEFT_DOWN
		};

		static std::string getHatPositionString(HatPosition hatPosition);

		virtual bool getButton(unsigned int index) const = 0;
		virtual float getAxis(unsigned int index) const = 0;
		virtual HatPosition getHat(unsigned int index) const = 0;

		virtual int getButtonCount() const = 0;
		virtual int getAxisCount() const = 0;
		virtual int getHatCount() const = 0;

		virtual void setDeadZone(unsigned int index, float deadZone) { (void)index; (void)deadZone; };

		Guid const &getGuid() const { return guid; }
		std::string const &getName() const { return name; }

	protected:
		Controller(int jid);

		// deadZone must be [0, 1[
		static float mapToDeadZone(float value, float deadZone);
	
		void setGuid(Guid const &guid);
		void setName(std::string const &name);

		virtual void reset() = 0;
		virtual void update() = 0;

		int jid;

	private:
		friend class Inputs;

		Guid guid;
		std::string name;

		int getJid() const { return jid; }
	};
}
