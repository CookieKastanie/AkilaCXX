#pragma once

#include <Akila/core/Core.hpp>

class GameLayer : public Akila::Layer {
private:
	Akila::ResourceReference<Akila::Mesh> sword;
	Akila::ResourceReference<Akila::Material> swordMaterial;

public:
	GameLayer();

	void update() override;
	void draw() override;
};
