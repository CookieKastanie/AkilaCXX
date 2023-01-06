#pragma once

#include "akila/core/resources/json.hpp"
#include "akila/engine/graphics/material.hpp"

namespace akila {
	class MaterialUniformParser {
	public:
		static void readUniforms(JSON const &json, Material *mat);
	};
}
