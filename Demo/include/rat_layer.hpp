#pragma once

#include <akila/akila.hpp>

class RatLayer: public akila::Layer {
public:
	RatLayer();

	void tick() override;
	void frame() override;
	void gui() override;

private:
	akila::Ref<akila::AudioSource> music;
};
