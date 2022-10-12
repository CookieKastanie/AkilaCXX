#pragma once

#include <akila/akila.hpp>
#include "render_system.hpp"

class TestLayer: public akila::Layer {
public:
	TestLayer();

	void tick() override;
	void frame() override;
	void gui() override;

private:
	akila::Ref<akila::Material> simpleMat;

	akila::Ref<akila::AudioSource> music;
	//akila::AudioEmitter boom;

	akila::Listener keyListener;
};
