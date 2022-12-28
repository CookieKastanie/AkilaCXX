#include "akila/engine/graphics/material_factory.hpp"
#include "akila/engine/tools/string_tools.hpp"
#include "akila/core/math/math.hpp"

using namespace akila;

std::string const MaterialFactory::SHADER_VERSION = "#version 450";

std::string const MaterialFactory::VERTEX_DELIMITER = "#akila_vertex";
std::string const MaterialFactory::GEOMETRIE_DELIMITER = "#akila_geometrie";
std::string const MaterialFactory::FRAGMENT_DELIMITER = "#akila_fragment";

std::string const MaterialFactory::INCLUDE_DIRECTIVE = "#akila_include";
std::string const MaterialFactory::TEMPLATE_DIRECTIVE = "#akila_template";
std::string const MaterialFactory::USER_CODE_DIRECTIVE = "#akila_user_code";
std::string const MaterialFactory::USE_TEMPLATE_DIRECTIVE = "#akila_use_template";

std::unordered_map<std::string, std::string> MaterialFactory::defines;
std::set<std::string> MaterialFactory::reservedUniforms;
std::unordered_map<std::string, MaterialFactory::ShaderSources> MaterialFactory::sources;
std::unordered_map<std::string, MaterialFactory::ShaderSources> MaterialFactory::templates;

////

std::string &MaterialFactory::ShaderSources::operator[](int index) {
	if(index == 0) return vertex;
	if(index == 1) return geometrie;
	return fragment;
}

int MaterialFactory::ShaderSources::size() {
	return 3;
}

////

// helper only used in MaterialFactory::setSource
std::size_t nextDelimiterIndex(std::size_t base, std::size_t a, std::size_t b, std::size_t strlen) {
	std::size_t da = a - base;
	std::size_t db = b - base;
	std::size_t dl = strlen - base;

	std::size_t next = strlen;
	std::size_t dmin = dl;

	if(a != -1 && dmin > da) {
		dmin = da;
		next = a;
	}

	if(b != -1 && dmin > db) {
		next = b;
	}
	
	return next - 1;
}

bool isMin(std::size_t base, std::size_t a, std::size_t b, std::size_t c) {
	if(base == -1) return false;

	if(base > a) return false;
	if(base > b) return false;
	if(base > c) return false;

	return true;
}

void MaterialFactory::setSource(std::string const &name, std::string const &source) {
	ShaderSources *sh = nullptr;

	std::size_t templateDirIndex = source.find(TEMPLATE_DIRECTIVE);
	std::size_t vertIndex = source.find(VERTEX_DELIMITER);
	std::size_t geomIndex = source.find(GEOMETRIE_DELIMITER);
	std::size_t fragIndex = source.find(FRAGMENT_DELIMITER);

	// if nothing specified, nothing to do
	if(templateDirIndex == -1 && vertIndex == -1 && geomIndex == -1 && fragIndex == -1) {
		sh = &sources[name];
		sh->unspecified = source;
		return;
	}

	// see if sources use template
	if(isMin(templateDirIndex, vertIndex, geomIndex, fragIndex)) {
		sh = &templates[name];
	} else {
		sh = &sources[name];

		// parse template name
		std::size_t useTemplateDirIndex = source.find(USE_TEMPLATE_DIRECTIVE);
		if(isMin(useTemplateDirIndex, vertIndex, geomIndex, fragIndex)) {
			std::size_t charIndex = useTemplateDirIndex + USE_TEMPLATE_DIRECTIVE.size();
			
			while(std::isspace(source[charIndex])) ++charIndex;
			std::size_t left = charIndex;

			while(std::isspace(source[charIndex]) == false) ++charIndex;
			std::size_t right = charIndex;
			
			sh->usedTemplateName = source.substr(left, right - left);
		}
	}
	
	// get vertex shader
	if(vertIndex != -1) {
		std::size_t endDelimiter = nextDelimiterIndex(vertIndex, geomIndex, fragIndex, source.size());
		std::size_t startDelimiter = vertIndex + VERTEX_DELIMITER.size();
		sh->vertex = source.substr(startDelimiter, endDelimiter - startDelimiter);
	} else {
		sh->vertex.clear();
	}

	// get geometrie shader
	if(geomIndex != -1) {
		std::size_t endDelimiter = nextDelimiterIndex(geomIndex, vertIndex, fragIndex, source.size());
		std::size_t startDelimiter = geomIndex + GEOMETRIE_DELIMITER.size();
		sh->geometrie = source.substr(startDelimiter, endDelimiter - startDelimiter);
	} else {
		sh->geometrie.clear();
	}

	// get fragment shader
	if(fragIndex != -1) {
		std::size_t endDelimiter = nextDelimiterIndex(fragIndex, vertIndex, geomIndex, source.size());
		std::size_t startDelimiter = fragIndex + FRAGMENT_DELIMITER.size();
		sh->fragment = source.substr(startDelimiter, endDelimiter - startDelimiter);
	} else {
		sh->fragment.clear();
	}
}

void MaterialFactory::define(std::string const &name, const std::string &value) {
	defines[name] = value;
}

void MaterialFactory::define(std::string const &name, float value) {
	defines[name] = std::to_string(value);
}

void MaterialFactory::define(std::string const &name, int value) {
	defines[name] = std::to_string(value);
}

void MaterialFactory::define(std::string const &name, unsigned int value) {
	defines[name] = std::to_string(value);
}

void MaterialFactory::define(std::string const &name, bool value) {
	defines[name] = std::to_string(value);
}

void MaterialFactory::reserveUniform(std::string const &name) {
	reservedUniforms.insert(name);
}

void MaterialFactory::recursiveInclude(
	std::string &out,
	std::string const &sourceName,
	std::set<std::string> &includeStack,
	int sourceIndex) {

	if(includeStack.find(sourceName) != includeStack.end()) return;
	includeStack.insert(sourceName);

	std::istringstream in{
		sources[sourceName].unspecified.empty() ?
		sources[sourceName][sourceIndex] : sources[sourceName].unspecified
	};
	std::string line;
	while(std::getline(in, line)) {
		std::size_t incIndex = line.find(INCLUDE_DIRECTIVE);
		if(incIndex != -1) {
			std::string incName = line.substr(incIndex + INCLUDE_DIRECTIVE.size(), line.size());
			StringTools::trim(incName);
			recursiveInclude(out, incName, includeStack, sourceIndex);
			continue;
		}

		for(auto &[name, value] : defines) {
			StringTools::replaceAll(line, name, value);
		}

		out += line;
		out += '\n';
	}
}

MaterialFactory::ShaderSources MaterialFactory::buildSources(std::string const &name) {
	std::string templateName = sources[name].usedTemplateName;

	ShaderSources sh{templateName, "", "", "", ""};

	std::set<std::string> includeStack;

	for(int i = 0; i < ShaderSources::size(); ++i) {
		includeStack.clear();

		if(templateName.empty() == false) {
			auto templateIt = templates.find(templateName);
			if(templateIt == templates.end()) {
				std::cerr << "Template : \"" + templateName + "\" do not exist.";
				return sh;
			}

			std::istringstream in{templateIt->second[i]};
			std::string line;
			bool userCodeIncluded = false;
			while(std::getline(in, line)) {
				if(userCodeIncluded == false && line.find(USER_CODE_DIRECTIVE) != -1) {
					userCodeIncluded = true;
					recursiveInclude(sh[i], name, includeStack, i);
					continue;
				}

				std::size_t incIndex = line.find(INCLUDE_DIRECTIVE);
				if(incIndex != -1) {
					std::string incName = line.substr(incIndex + INCLUDE_DIRECTIVE.size(), line.size());
					StringTools::trim(incName);
					recursiveInclude(sh[i], incName, includeStack, i);
					continue;
				}

				for(auto &[name, value] : defines) {
					StringTools::replaceAll(line, name, value);
				}

				sh[i] += line;
				sh[i] += '\n';
			}

		} else {
			recursiveInclude(sh[i], name, includeStack, i);
		}
	}

	if(sh.vertex.empty() == false) {
		sh.vertex = (SHADER_VERSION + "\n") + sh.vertex;
	}

	if(sh.geometrie.empty() == false) {
		sh.geometrie = (SHADER_VERSION + "\n") + sh.geometrie;
	}

	if(sh.fragment.empty() == false) {
		sh.fragment = (SHADER_VERSION + "\n") + sh.fragment;
	}

	return sh;
}

Ref<Material> MaterialFactory::build(std::string const &name) {
	ShaderSources sh = buildSources(name);
	return Resources::set<Material>(name, new Material{sh.vertex, sh.geometrie, sh.fragment, reservedUniforms});
}
