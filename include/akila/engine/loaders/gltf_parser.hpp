#pragma once

#include "akila/core/resources/json.hpp"
#include "akila/core/math/math.hpp"
#include <vector>
#include <string>
#include <unordered_map>
#include "akila/core/rhi/buffer_object.hpp"
#include "akila/engine/graphics/mesh.hpp"

namespace akila {
	struct GlTF {
		std::string name;

		Vec3 min = {0.f, 0.f, 0.f};
		Vec3 max = {0.f, 0.f, 0.f};
		float radius = 0.f;
		float squaredRadius = 0.f;

		struct Buffer {
			BufferObject::Type componentType = BufferObject::Type::FLOAT;
			int componentTypeCount = 0;
			std::size_t count = 0;
			void *data = nullptr;
			std::size_t byteCount = 0;
		};

		std::unordered_map<Mesh::Attributes, Buffer> attributes;
		Buffer indices;
	};

	class GlTFParser {
	public:
		GlTFParser();
		~GlTFParser() = default;

		GlTFParser(GlTFParser &&other) noexcept = delete;
		GlTFParser &operator=(GlTFParser &&other) noexcept = delete;

		GlTFParser(GlTFParser const &other) = delete;
		GlTFParser &operator=(GlTFParser const &other) = delete;

		void setInvertTexcoord(bool invert);
		bool loadFile(std::string const &path);
		std::vector<GlTF> const &getResult();

	private:
		std::vector<std::uint8_t> raw;
		std::vector<GlTF> result;
		bool invertTexcoord;

		void positionSpecialCase(GlTF &mesh, JSON const &accessorJson);
		void tangentSpecialCase(GlTF::Buffer &buffer);
		void texcoordSpecialCase(GlTF::Buffer &buffer);
	};
}
