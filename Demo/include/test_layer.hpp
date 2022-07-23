#pragma once

#include <akila/akila.hpp>

class TestLayer: public akila::Layer {
public:
	TestLayer();

	void tick() override;
	void frame() override;
	void gui() override;

private:
	akila::Listener keyListener;
	akila::Ref<akila::Material> simpleMat;
};
