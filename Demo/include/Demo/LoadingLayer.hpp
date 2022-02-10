#pragma once

#include "Akila/core/Core.hpp"
#include "Demo/MouseCamera.hpp"

class LoadingLayer: public Akila::Layer {
private:
	Akila::ResourceReference<Akila::Shader> shader;
	Akila::ResourceReference<Akila::Mesh> triangle;

public:
	LoadingLayer();

	virtual void update() override;
	virtual void draw() override;
};