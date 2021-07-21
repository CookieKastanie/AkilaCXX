#pragma once

#include "Akila/graphics/gl/Texture.hpp"
#include <memory>

namespace Akila {
	class Environment {
	private:
	public:
		static std::shared_ptr<Texture> Environment::createBRDFLUT();
	};
}
