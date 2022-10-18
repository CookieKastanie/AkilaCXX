#pragma once

#include <akila/akila.hpp>
#include <akila/default/resources.hpp>
#include <akila/default/loaders.hpp>

class Kitchen {
public:
	static void loadShaders();
	static void bakeBrdfLut(float texSize = 128);
	static void bakeCubemapFromEqui(akila::Texture2D const *tex, akila::IVec2 size = {1024, 1024});
};