#pragma once

#include "Akila/core/Core.hpp"
#include "Akila/core/State.hpp"
#include "Akila/graphics/Material.hpp"
#include "Akila/graphics/Mesh.hpp"

class LoadingState: public Akila::State {
	public:
		std::shared_ptr<Akila::Mesh> defaultTriangle;

	public:
		LoadingState();

		virtual void update() override;
		virtual void draw() override;
};