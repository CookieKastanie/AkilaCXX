#pragma once

#include <memory>
#include "Akila/core/State.hpp"
#include "Akila/graphics/gl/Shader.hpp"
#include "Akila/graphics/gl/VAO.hpp"
#include "Akila/graphics/gl/Buffer.hpp"

class LoadingState: public Akila::State {
	public:
		std::shared_ptr<Akila::VAO> vao;
		std::shared_ptr<Akila::VBO> vertex;
		std::shared_ptr<Akila::VBO> uv;


	public:
		LoadingState();

		virtual void update() override;
		virtual void draw() override;
};