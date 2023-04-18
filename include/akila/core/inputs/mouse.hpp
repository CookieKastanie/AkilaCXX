#pragma once

#include "akila/core/inputs/controller.hpp"
#include "akila/core/math/math.hpp"
#include <array>

namespace akila {
	class Mouse: public Controller {
	public:
		enum class Button: int {
			LEFT_CLICK = GLFW_MOUSE_BUTTON_1,
			RIGHT_CLICK = GLFW_MOUSE_BUTTON_2,
			MIDDLE_CLICK = GLFW_MOUSE_BUTTON_3,

			BUTTON_1 = GLFW_MOUSE_BUTTON_1,
			BUTTON_2 = GLFW_MOUSE_BUTTON_2,
			BUTTON_3 = GLFW_MOUSE_BUTTON_3,
			BUTTON_4 = GLFW_MOUSE_BUTTON_4,
			BUTTON_5 = GLFW_MOUSE_BUTTON_5,
			BUTTON_6 = GLFW_MOUSE_BUTTON_6,
			BUTTON_7 = GLFW_MOUSE_BUTTON_7,
			BUTTON_8 = GLFW_MOUSE_BUTTON_8
		};

		bool getButton(unsigned int index) const override { return buttons[index]; }
		float getAxis(unsigned int index) const override { (void)index; return 0.f; }
		HatPosition getHat(unsigned int index) const override { (void)index; return HatPosition::CENTERED; }

		bool getButton(Button button) const { return buttons[static_cast<int>(button)]; }

		int getButtonCount() const override { return GLFW_MOUSE_BUTTON_LAST + 1; }
		int getAxisCount() const override { return 0; }
		int getHatCount() const override { return 0; }

		Vec2 const &getPosition() const { return position; }
		Vec2 const &getClipSpacePosition() const { return clipSpacePosition; }
		Vec2 const &getVelocity() const { return velocity; }
		Vec2 const &getScrollVelocity() const { return scrollVelocity; }

	private:
		friend class Inputs;

		Mouse();
		void reset() override;
		void update() override;

		void setButton(Button button, bool state) { buttons[static_cast<int>(button)] = state; }
		void setPosition(Vec2 const &p) { position = p; }
		void setClipSpacePosition(Vec2 const &p) { clipSpacePosition = p; }
		void setVelocity(Vec2 const &v) { velocity = v; }
		void setScrollVelocity(Vec2 const &v) { scrollVelocity = v; }

		std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> buttons;

		Vec2 position{0.f};
		Vec2 clipSpacePosition{0.f};
		Vec2 velocity{0.f};
		Vec2 scrollVelocity{0.f};
	};
}
