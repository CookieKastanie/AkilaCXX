#pragma once

#include "Akila/core/Core.hpp"
#include "Akila/core/Layer.hpp"
#include "Akila/graphics/Material.hpp"
#include "Akila/graphics/Mesh.hpp"
#include "Demo/MouseCamera.hpp"

class LoadingLayer: public Akila::Layer {
	public:
		std::shared_ptr<Akila::Mesh> defaultTriangle;

	public:
		LoadingLayer();

		virtual void update() override;
		virtual void draw() override;
};