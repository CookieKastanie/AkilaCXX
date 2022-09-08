#pragma once

#include <akila/akila.hpp>

class UtilsLayer: public akila::Layer {
public:
	UtilsLayer();

	void tick() override;
	void frame() override;
	void gui() override;

private:
	akila::Listener keyListener;
	bool isLoading;
};
