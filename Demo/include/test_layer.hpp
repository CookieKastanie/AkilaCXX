#pragma once

#include <akila/akila.hpp>

class TestLayer: public akila::Layer {
public:
	TestLayer();

	void update() override;
	void draw() override;
	void drawImGui() override;
};
