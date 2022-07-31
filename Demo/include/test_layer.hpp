#pragma once

#include <akila/akila.hpp>

class TestLayer: public akila::Layer {
public:
	TestLayer();

	void tick() override;
	void frame() override;
	void gui() override;

private:
	akila::Ref<akila::Material> simpleMat;

	akila::Listener keyListener;

	std::vector<akila::AudioEmitter> emitters;
};
