#pragma once

#include <string>
#include "akila/core/rhi/shader.hpp"
#include "akila/core/resources/resources.hpp"
#include <unordered_map>

namespace akila {
	class ShaderFactory {
	public:
		static std::string const VERTEX_DELIMITER;
		static std::string const GEOMETRIE_DELIMITER;
		static std::string const FRAGMENT_DELIMITER;

		static std::string const INCLUDE_DIRECTIVE;

		template<typename T>
		static void define(std::string const &name, T const &value) {
			defines[name] = std::to_string(value);
		}

		static void setSource(std::string const &name, std::string const &source);

		struct ShaderSources {
			std::string vertex;
			std::string geometrie;
			std::string fragment;
		};

		static ShaderSources buildSources(std::string const &name);
		static Ref<Shader> build(std::string const &name);

	private:
		static std::unordered_map<std::string, std::string> defines;
		static std::unordered_map<std::string, ShaderSources> sources;
	};
}
