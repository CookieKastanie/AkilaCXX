#include "akila/engine/loaders/gltf_parser.hpp"
#include <fstream>

using namespace akila;

Mesh::Attributes nameToAttribute(std::string const &name) {
	if(name == "POSITION")   return Mesh::Attributes::POSITION;
	if(name == "NORMAL")     return Mesh::Attributes::NORMAL;
	if(name == "TEXCOORD_0") return Mesh::Attributes::TEXCOORD_0;
	if(name == "TEXCOORD_1") return Mesh::Attributes::TEXCOORD_1;
	if(name == "TEXCOORD_2") return Mesh::Attributes::TEXCOORD_2;
	if(name == "TANGENT")    return Mesh::Attributes::TANGENT;
	if(name == "COLOR_0")    return Mesh::Attributes::COLOR_0;
	if(name == "COLOR_1")    return Mesh::Attributes::COLOR_1;
	if(name == "COLOR_2")    return Mesh::Attributes::COLOR_2;
	if(name == "JOINTS_0")   return Mesh::Attributes::JOINTS_0;
	if(name == "JOINTS_1")   return Mesh::Attributes::JOINTS_1;
	if(name == "JOINTS_2")   return Mesh::Attributes::JOINTS_2;
	if(name == "WEIGHTS_0")  return Mesh::Attributes::WEIGHTS_0;
	if(name == "WEIGHTS_1")  return Mesh::Attributes::WEIGHTS_1;
	if(name == "WEIGHTS_2")  return Mesh::Attributes::WEIGHTS_2;
	return Mesh::Attributes::ANY;
}

int nameToComponentCount(std::string const &name) {
	if(name == "SCALAR") return 1;
	if(name == "VEC2")   return 2;
	if(name == "VEC3")   return 3;
	if(name == "VEC4")   return 4;
	if(name == "MAT2")   return 4;
	if(name == "MAT3")   return 9;
	return 16; // MAT4
}

GlTFParser::GlTFParser(): invertTexcoord{true} {

}

void GlTFParser::setInvertTexcoord(bool invert) {
	invertTexcoord = invert;
}

std::vector<GlTF> const &GlTFParser::getResult() {
	return result;
}

bool GlTFParser::loadFile(std::string const &path) {
	result.clear();

	std::ifstream file(path, std::ios::binary);
	if(!file.good()) {
		std::cerr << "GlTF loading error : can't read " << path << std::endl;
		return false;
	}

	{
		std::uint32_t val = 0;

		file.read(reinterpret_cast<char *>(&val), sizeof(val));
		if(val != 0x46546C67) { // "glTF"
			std::cerr << "GlTF bad format : can't read " << path << std::endl;
			return false;
		}

		file.read(reinterpret_cast<char *>(&val), sizeof(val));
		// verifier la version ?
		//std::cout << "version : " << val << std::endl;

		file.read(reinterpret_cast<char *>(&val), sizeof(val));
		// verifier la longueur a la fin ?
		//std::cout << "length : " << val << std::endl;
	}

	JSON json;
	{ // le premier block doit etre de type JSON
		std::uint32_t chunkLength = 0;
		file.read(reinterpret_cast<char *>(&chunkLength), sizeof(chunkLength));
		std::uint32_t chunkType = 0;
		file.read(reinterpret_cast<char *>(&chunkType), sizeof(chunkType));

		if(chunkType != 0x4E4F534A) { // "JSON"
			std::cerr << "GlTF bad format : can't read " << path << std::endl;
			return false;
		}

		std::string jsonText;
		jsonText.resize(chunkLength);
		file.read(jsonText.data(), chunkLength);
		json = JSON::parse(jsonText);
	}

	{ // le second de type BIN
		std::uint32_t chunkLength = 0;
		file.read(reinterpret_cast<char *>(&chunkLength), sizeof(chunkLength));
		std::uint32_t chunkType = 0;
		file.read(reinterpret_cast<char *>(&chunkType), sizeof(chunkType));

		if(chunkType != 0x004E4942) { // "BIN"
			std::cerr << "GlTF bad format : can't read " << path << std::endl;
			return false;
		}

		raw.resize(chunkLength);
		file.read(reinterpret_cast<char *>(raw.data()), chunkLength);
	}

	// parsing
	if(
		json["meshes"].is_array() == false ||
		json["accessors"].is_array() == false ||
		json["bufferViews"].is_array() == false) {
		std::cerr << "Malformed GlTF : can't read " << path << std::endl;
		return false;
	}

	JSON const &bufferViewsJson = json["bufferViews"];
	JSON const &accessorsJson = json["accessors"];

	for(JSON const &meshJson : json["meshes"]) {
		GlTF &mesh = result.emplace_back();
		mesh.name = meshJson["name"];

		for(auto const &attrJson : meshJson["primitives"][0]["attributes"].items()) {
			std::string const &attrName = attrJson.key();
			std::size_t accessorsIndex = attrJson.value();

			Mesh::Attributes attribute = nameToAttribute(attrName);
			GlTF::Buffer &buffer = mesh.attributes[attribute];
			
			JSON const &accessorJson = accessorsJson[accessorsIndex];
			std::size_t bufferViewIndex = accessorJson["bufferView"];
			JSON const &bufferViewJson = bufferViewsJson[bufferViewIndex];

			buffer.count = accessorJson["count"];
			buffer.componentTypeCount = nameToComponentCount(accessorJson["type"]);
			buffer.componentType = accessorJson["componentType"];
			buffer.byteCount = bufferViewJson["byteLength"];
			buffer.data = raw.data() + static_cast<std::size_t>(bufferViewJson["byteOffset"]);

			// check special cases
			switch(attribute) {
				case Mesh::Attributes::POSITION:
					positionSpecialCase(mesh, accessorJson);
					break;

				case Mesh::Attributes::TANGENT:
					tangentSpecialCase(buffer);
					break;

				case Mesh::Attributes::TEXCOORD_0:
				case Mesh::Attributes::TEXCOORD_1:
				case Mesh::Attributes::TEXCOORD_2:
					texcoordSpecialCase(buffer);
					break;
			}
		}

		if(meshJson["primitives"][0]["indices"].is_number()) {
			std::size_t accessorsIndex = meshJson["primitives"][0]["indices"];

			GlTF::Buffer &buffer = mesh.indices;

			JSON const &accessorJson = accessorsJson[accessorsIndex];
			std::size_t bufferViewIndex = accessorJson["bufferView"];
			JSON const &bufferViewJson = bufferViewsJson[bufferViewIndex];

			buffer.count = accessorJson["count"];
			buffer.componentTypeCount = nameToComponentCount(accessorJson["type"]);
			buffer.componentType = accessorJson["componentType"];
			buffer.byteCount = bufferViewJson["byteLength"];
			buffer.data = raw.data() + static_cast<std::size_t>(bufferViewJson["byteOffset"]);
		}
	}

	return true;
}

void GlTFParser::positionSpecialCase(GlTF &mesh, JSON const &accessorJson) {
	mesh.min.x = accessorJson["min"][0];
	mesh.min.y = accessorJson["min"][1];
	mesh.min.z = accessorJson["min"][2];

	mesh.max.x = accessorJson["max"][0];
	mesh.max.y = accessorJson["max"][1];
	mesh.max.z = accessorJson["max"][2];

	mesh.radius = max(abs(mesh.max.x), max(abs(mesh.max.y), abs(mesh.max.z)));
	mesh.radius = max(mesh.radius, max(abs(mesh.min.x), max(abs(mesh.min.y), abs(mesh.min.z))));

	mesh.squaredRadius = mesh.radius * mesh.radius;
}

void GlTFParser::tangentSpecialCase(GlTF::Buffer &buffer) {
	// supprime la 4eme dimention des tengantes
	buffer.componentTypeCount = 3;
	Vec3 *vec3View = reinterpret_cast<Vec3 *>(buffer.data);
	Vec4 *vec4View = reinterpret_cast<Vec4 *>(buffer.data);

	std::size_t count = 0;
	for(std::size_t byteCount = 0; byteCount < buffer.byteCount; byteCount += sizeof(Vec4)) {
		memcpy(vec3View, vec4View, sizeof(Vec3));
		++vec3View;
		++vec4View;
		++count;
	}

	buffer.byteCount = count * sizeof(Vec3);
}

void GlTFParser::texcoordSpecialCase(GlTF::Buffer &buffer) {
	if(invertTexcoord == false) return;

	Vec2 *vec2View = reinterpret_cast<Vec2 *>(buffer.data);
	for(std::size_t byteCount = 0; byteCount < buffer.byteCount; byteCount += sizeof(Vec2)) {
		Vec2 *v = vec2View;
		v->y = 1.f - v->y;
		++vec2View;
	}
}
