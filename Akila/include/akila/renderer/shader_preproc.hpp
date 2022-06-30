#pragma once

#include <string>

namespace akila {
	class ShaderPreProc {
	public:
		struct ShaderSources {
			std::string vertexShader;
			std::string fragmentShader;
			std::string geometryShader;
		};

		static std::string version;

		static void process(std::string const &source, ShaderSources &sources, std::string const &currentPath = "");
	};
}
