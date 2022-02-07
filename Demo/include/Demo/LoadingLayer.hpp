#pragma once

#include "Akila/core/Core.hpp"
//#include "Akila/core/Layer.hpp"
//#include "Akila/graphics/gl/Shader.hpp"
//#include "Akila/graphics/Material.hpp"
//#include "Akila/graphics/Mesh.hpp"
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