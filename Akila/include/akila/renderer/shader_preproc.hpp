#pragma once

#include <string>
#include <set>

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

		template<typename T>
		static void define(std::string const &name, T const &value) {
			defines.insert({name, std::to_string(value)});
		}

	private:
		struct Define {
			std::string name;
			std::string value;
			bool operator<(Define const &other) const;
		};
		static std::set<Define> defines;
	};
}
