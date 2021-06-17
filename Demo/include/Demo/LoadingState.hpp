#pragma once

#include "Akila/core/State.hpp"

class LoadingState: public Akila::State {
	public:
		LoadingState();

		virtual void update() override;
		virtual void draw() override;
};