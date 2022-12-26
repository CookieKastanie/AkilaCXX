#include "akila/engine/graphics/shader_factory.hpp"
#include "akila/engine/tools/string_tools.hpp"

using namespace akila;

std::string const ShaderFactory::VERTEX_DELIMITER = "#akila_vertex";
std::string const ShaderFactory::GEOMETRIE_DELIMITER = "#akila_geometrie";
std::string const ShaderFactory::FRAGMENT_DELIMITER = "#akila_fragment";

std::string const ShaderFactory::INCLUDE_DIRECTIVE = "#akila_include";

std::unordered_map<std::string, std::string> ShaderFactory::defines;
std::unordered_map<std::string, ShaderFactory::ShaderSources> ShaderFactory::sources;

// helper only used in ShaderFactory::setSource
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

void ShaderFactory::setSource(std::string const &name, std::string const &source) {
	// separate source into 3
	std::size_t vertIndex = source.find(VERTEX_DELIMITER);
	std::size_t geomIndex = source.find(GEOMETRIE_DELIMITER);
	std::size_t fragIndex = source.find(FRAGMENT_DELIMITER);

	ShaderSources &sh = sources[name];

	if(vertIndex != -1) {
		std::size_t endDelimiter = nextDelimiterIndex(vertIndex, geomIndex, fragIndex, source.size());
		std::size_t startDelimiter = vertIndex + VERTEX_DELIMITER.size();
		sh.vertex = source.substr(startDelimiter, endDelimiter - startDelimiter);
	} else {
		sh.vertex.clear();
	}

	if(geomIndex != -1) {
		std::size_t endDelimiter = nextDelimiterIndex(geomIndex, vertIndex, fragIndex, source.size());
		std::size_t startDelimiter = geomIndex + GEOMETRIE_DELIMITER.size();
		sh.geometrie = source.substr(startDelimiter, endDelimiter - startDelimiter);
	} else {
		sh.geometrie.clear();
	}

	if(fragIndex != -1) {
		std::size_t endDelimiter = nextDelimiterIndex(fragIndex, vertIndex, geomIndex, source.size());
		std::size_t startDelimiter = fragIndex + FRAGMENT_DELIMITER.size();
		sh.fragment = source.substr(startDelimiter, endDelimiter - startDelimiter);
	} else {
		sh.fragment.clear();
	}
}

ShaderFactory::ShaderSources ShaderFactory::buildSources(std::string const &name) {
	ShaderSources sh;



	return sh;
}

Ref<Shader> ShaderFactory::build(std::string const &name) {
	ShaderSources sh = buildSources(name);
	return Resources::create<Shader>(name, sh.vertex, sh.geometrie, sh.fragment);
}
