#pragma once

#include <akila/akila.hpp>

class TestLayer: public akila::Layer {
public:
	akila::Listener l;
	akila::Listener keyPressListener;

	TestLayer();

	void update() override;
	void draw() override;
	void drawImGui() override;
};
