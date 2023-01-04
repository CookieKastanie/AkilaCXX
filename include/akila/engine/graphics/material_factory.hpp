#pragma once

#include <string>
#include "akila/engine/graphics/material.hpp"
#include "akila/core/resources/resources.hpp"
#include <unordered_map>
#include <set>
#include <sstream>
#include <iostream>

namespace akila {
	class MaterialFactory {
	public:
		static std::string const SHADER_VERSION;

		static std::string const VERTEX_DELIMITER;
		static std::string const GEOMETRIE_DELIMITER;
		static std::string const FRAGMENT_DELIMITER;

		static std::string const INCLUDE_DIRECTIVE;
		static std::string const TEMPLATE_DIRECTIVE;
		static std::string const USER_CODE_DIRECTIVE;
		static std::string const USE_TEMPLATE_DIRECTIVE;

		static void define(std::string const &name, const std::string &value);
		static void define(std::string const &name, float value);
		static void define(std::string const &name, int value);
		static void define(std::string const &name, unsigned int value);
		static void define(std::string const &name, bool value);

		/* Materials will not see reserved uniforms */
		static void reserveUniform(std::string const &name);

		static void setSource(std::string const &name, std::string const &source);

		struct ShaderSources {
			std::string usedTemplateName;
			std::string unspecified;
			std::string vertex;
			std::string geometrie;
			std::string fragment;

			std::string &operator[](int index);
			static int size();
		};

		static ShaderSources buildSources(std::string const &name);
		static Ref<Material> build(std::string const &matName, std::string const &sourceName);

	private:
		static std::unordered_map<std::string, std::string> defines;
		static std::set<std::string> reservedUniforms;
		static std::unordered_map<std::string, ShaderSources> sources;
		static std::unordered_map<std::string, ShaderSources> templates;

		static void recursiveInclude(
			std::string &out,
			std::string const &sourceName,
			std::set<std::string> &includeStack,
			int sourceIndex);
	};
}
