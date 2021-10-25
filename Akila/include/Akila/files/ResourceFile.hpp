#pragma once

#include <vector>
#include <string>
#include "Akila/graphics/gl/Shader.hpp"
#include "Akila/graphics/gl/Texture.hpp"
#include "Akila/graphics/Material.hpp"
#include "Akila/graphics/Mesh.hpp"

namespace Akila {
	class ResourceFile {
	public:
		struct ShaderUnifInfos {
			std::string name;
			bool isInt;
			Material::UniformValue value;
		};

		struct ShaderInfos {
			std::string path;
			std::string name;
			std::vector<ShaderUnifInfos> unifsInfos;
		};

		struct TextureInfos {
			std::string name;
			std::string src;
			TextureBuffer::Format format;
			TextureBuffer::Parameters parameters;
			bool generateMips;
		};

		struct MeshInfos {
			std::string name;
			std::string src;
		};

		struct MaterialInfos {
			std::string name;
			std::string shader;
			std::vector<ShaderUnifInfos> unifsInfos;
			std::vector<Material::TextureBinding> textureBindings;
		};

		std::vector<ShaderInfos> shaders;
		std::vector<TextureInfos> textures;
		std::vector<MeshInfos> meshs;
		std::vector<MaterialInfos> materials;

		void unserializeFrom(const std::string path);
	};
}
