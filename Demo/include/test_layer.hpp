#pragma once

#include <akila/akila.hpp>

class TestLayer: public akila::Layer {
public:
	int a;
	float x;
	float oldX;

	akila::Listener l;
	akila::Listener keyPressListener;

	TestLayer();

	void update() override;
	void draw() override;
	void drawImGui() override;
};
