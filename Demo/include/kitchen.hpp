#pragma once

#include <akila/akila.hpp>
#include <akila/default/resources.hpp>
#include <akila/default/loaders.hpp>

class Kitchen {
public:
	static void loadShaders();
	static void bakeBrdfLut(std::string const &name, float texSize = 128);
	static void bakeCubemapFromEqui(std::string const &name, akila::Texture2D const *tex, akila::IVec2 size = {1024, 1024});
	static void bakeIrradiance(std::string const &name, akila::TextureCubmap const *skybox, akila::IVec2 size = {32, 32});
	static void bakePrefilter(std::string const &name, akila::TextureCubmap const *skybox, akila::IVec2 size = {128, 128});
};