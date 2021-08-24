#pragma once

#include "Akila/core/Core.hpp"
#include "Akila/core/State.hpp"
#include "Akila/graphics/Material.hpp"
#include "Akila/graphics/Mesh.hpp"
#include "Demo/MouseCamera.hpp"

class PlayState: public Akila::State {
public:
	PlayState();

	virtual void update() override;
	virtual void draw() override;
};