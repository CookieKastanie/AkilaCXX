#pragma once

#include "akila/core/memory/ref.hpp"
#include "akila/core/rhi/shader.hpp"
#include "akila/core/math/math.hpp"
#include "akila/core/rhi/texture.hpp"
#include "akila/engine/containers/small_vector.hpp"
#include <unordered_map>
#include <set>

namespace akila {
	namespace internal {
		class MaterialContainer {
		protected:
			SmallVector<std::uint8_t, 512> uniformData;
			
			struct TextureBinding {
				std::string name = "";
				int unit = 0;
				Ref<TextureBuffer> textureBuffer = Ref<TextureBuffer>{};
			};
			SmallVector<TextureBinding, 32> textures;

			void writeRaw(UniformInfos const &infos, void const *data, std::size_t byteCount);
		};
	}

	class Material: public internal::MaterialContainer {
	public:
		using Id = std::size_t;

		Material();

		void write(std::string const &name, int data);
		void write(std::string const &name, float data);

		void write(std::string const &name, Vec2 const &data);
		void write(std::string const &name, std::vector<Vec2> const &data);

		void write(std::string const &name, Vec3 const &data);
		void write(std::string const &name, std::vector<Vec3> const &data);

		void write(std::string const &name, Vec4 const &data);
		void write(std::string const &name, std::vector<Vec4> const &data);

		void write(std::string const &name, Mat4 const &data);
		void write(std::string const &name, std::vector<Mat4> const &data);

		template<typename T>
		T const &read(std::string const &name) {
			auto it = uniformsNamesToIndex.find(name);
			if(it == uniformsNamesToIndex.end()) {
				return *reinterpret_cast<T*>(uniformData.data());
			};

			return *reinterpret_cast<T*>(uniformData.data() + uniformDescriptors[it->second].byteOffset);
		}

		void affect(std::string const &name, Ref<TextureBuffer> texRef);

		void send();

		void sendReserved(std::string const &name, int data);
		void sendReserved(std::string const &name, float data);

		void sendReserved(std::string const &name, Vec2 const &data);
		void sendReserved(std::string const &name, std::vector<Vec2> const &data);

		void sendReserved(std::string const &name, Vec3 const &data);
		void sendReserved(std::string const &name, std::vector<Vec3> const &data);

		void sendReserved(std::string const &name, Vec4 const &data);
		void sendReserved(std::string const &name, std::vector<Vec4> const &data);

		void sendReserved(std::string const &name, Mat4 const &data);
		void sendReserved(std::string const &name, std::vector<Mat4> const &data);

	private:
		friend class MaterialFactory;
		friend class MaterialInstance;

		Material(
			std::string const &vertexShader,
			std::string const &geometrieShader,
			std::string const &fragmentShader,
			std::set<std::string> const &reservedUniforms
		);

		static Id nextId;
		static Id lastSended;
		Id id;

		Shader shader;

		SmallVector<UniformInfos, 16> reservedUniformDescriptors;
		SmallVector<UniformInfos, 16> uniformDescriptors;

		std::unordered_map<std::string, std::size_t> reservedUniformsNamesToIndex;
		std::unordered_map<std::string, std::size_t> uniformsNamesToIndex;
		std::unordered_map<std::string, std::size_t> texturesNamesToIndex;
	};

	class MaterialInstance: public internal::MaterialContainer {
	public:
		MaterialInstance(Ref<Material> mat);

		void write(std::string const &name, int data);
		void write(std::string const &name, float data);

		void write(std::string const &name, Vec2 const &data);
		void write(std::string const &name, std::vector<Vec2> const &data);

		void write(std::string const &name, Vec3 const &data);
		void write(std::string const &name, std::vector<Vec3> const &data);

		void write(std::string const &name, Vec4 const &data);
		void write(std::string const &name, std::vector<Vec4> const &data);

		void write(std::string const &name, Mat4 const &data);
		void write(std::string const &name, std::vector<Mat4> const &data);

		template<typename T>
		T const &read(std::string const &name) {
			auto const &uniformsNamesToIndex = material->uniformsNamesToIndex;

			auto it = uniformsNamesToIndex.find(name);
			if(it == uniformsNamesToIndex.end()) {
				return *reinterpret_cast<T *>(uniformData.data());
			};

			return *reinterpret_cast<T *>(
				uniformData.data() + material->uniformDescriptors[it->second].byteOffset
			);
		}

		void affect(std::string const &name, Ref<TextureBuffer> texRef);

		void send();

	private:
		Ref<Material> material;

		// smallvector do not work with bool wtf
		SmallVector<std::uint8_t, 16> overridedUniforms;
		SmallVector<std::uint8_t, 32> overridedTextures;
	};
}
