#pragma once

#include <Akila/core/Core.hpp>

class GameLayer : public Akila::Layer {
private:
	Akila::Ref<Akila::Mesh> sword;
	Akila::Ref<Akila::Material> swordMaterial;

public:
	GameLayer();

	void update() override;
	void draw() override;
	void drawImGui() override;
};
