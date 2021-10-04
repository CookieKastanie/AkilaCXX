#pragma once

#include "Akila/core/Core.hpp"
#include "Akila/core/Layer.hpp"
#include "Akila/graphics/Material.hpp"
#include "Akila/graphics/Mesh.hpp"
#include "Demo/MouseCamera.hpp"

#include "Demo/Net.hpp"

class PlayLayer: public Akila::Layer {
private:
	float exposure;
	Net net;

public:
	PlayLayer();

	virtual void update() override;
	virtual void draw() override;
};