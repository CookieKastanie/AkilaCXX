#pragma once

#include "akila/core/resources/json.hpp"
#include "akila/engine/graphics/material.hpp"

namespace akila {
	class MaterialParser {
	public:
		static void populateUniforms(JSON &json, internal::MaterialContainer *mat);
		static void populateTextures(JSON &json, internal::MaterialContainer *mat);
	};
}
