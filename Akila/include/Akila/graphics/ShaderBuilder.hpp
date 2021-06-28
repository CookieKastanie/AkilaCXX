#pragma once

#include <string>
#include <memory>
#include "Akila/graphics/gl/Shader.hpp"

namespace Akila {
	class ShaderBuilder {
		private:
			static const std::string version;
			static const std::string attributes;
			static const std::string uniforms;
			static const std::string functions;

		public:
			static const std::string vertDelimiter;
			static const std::string fragDelimiter;
			static const std::string geoDelimiter;

			enum Attributes: unsigned int {
				A_POSITION = 0,
				A_UV = 1,
				A_NORMAL = 2,
				A_TANGENT = 3,
				A_COLOR = 4
			};

			static std::shared_ptr<Shader> build(const std::string &source);
			static std::shared_ptr<Shader> buildFromFile(const std::string &path);
	};
}
