#pragma once

#include <akila/akila.hpp>
#include "render_system.hpp"

class RatLayer: public akila::Layer {
public:
	void onMount() override;
	void onUnmount() override;

	void tick() override;
	void frame() override;
	void gui() override;

private:
	RenderSystem *renderSystem;

	akila::Listener keyListener;
	//akila::Ref<akila::AudioSource> music;
};
