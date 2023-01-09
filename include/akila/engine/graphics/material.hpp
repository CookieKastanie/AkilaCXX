#pragma once

#include "akila/core/memory/ref.hpp"
#include "akila/core/rhi/shader.hpp"
#include "akila/core/math/math.hpp"
#include "akila/core/rhi/texture.hpp"
#include "akila/engine/containers/small_vector.hpp"
#include <unordered_map>
#include <vector>
#include <string>
#include <set>

namespace akila {
	namespace internal {
		class MaterialContainer {
		public:
			struct TextureBinding {
				std::string name = "";
				int unit = 0;
				Ref<TextureBuffer> textureBuffer = Ref<TextureBuffer>{};
			};

			virtual void write(std::string const &name, int data) = 0;
			virtual void write(std::string const &name, std::vector<int> const &data) = 0;

			virtual void write(std::string const &name, unsigned int data) = 0;
			virtual void write(std::string const &name, std::vector<unsigned int> const &data) = 0;

			virtual void write(std::string const &name, float data) = 0;
			virtual void write(std::string const &name, std::vector<float> const &data) = 0;

			virtual void write(std::string const &name, Vec2 const &data) = 0;
			virtual void write(std::string const &name, std::vector<Vec2> const &data) = 0;

			virtual void write(std::string const &name, Vec3 const &data) = 0;
			virtual void write(std::string const &name, std::vector<Vec3> const &data) = 0;

			virtual void write(std::string const &name, Vec4 const &data) = 0;
			virtual void write(std::string const &name, std::vector<Vec4> const &data) = 0;

			virtual void write(std::string const &name, Mat4 const &data) = 0;
			virtual void write(std::string const &name, std::vector<Mat4> const &data) = 0;

			virtual SmallVector<UniformInfos, 16> const &getUniformInfos() = 0;
			SmallVector<TextureBinding, 32> const &getTextureBindings();

			virtual ~MaterialContainer() = default;

		protected:
			// empty memory space used when user read unvalid material memory
			static std::array<std::uint8_t, 512> const voidMaterialMemory;

			SmallVector<std::uint8_t, 512> uniformData;
			SmallVector<TextureBinding, 32> textures;

			void writeRaw(UniformInfos const &infos, void const *data, std::size_t byteCount);
		};
	}

	class Material: public internal::MaterialContainer {
	public:
		using Id = std::size_t;

		Material();

		void write(std::string const &name, int data) override;
		void write(std::string const &name, std::vector<int> const &data) override;

		void write(std::string const &name, unsigned int data) override;
		void write(std::string const &name, std::vector<unsigned int> const &data) override;

		void write(std::string const &name, float data) override;
		void write(std::string const &name, std::vector<float> const &data) override;

		void write(std::string const &name, Vec2 const &data) override;
		void write(std::string const &name, std::vector<Vec2> const &data) override;

		void write(std::string const &name, Vec3 const &data) override;
		void write(std::string const &name, std::vector<Vec3> const &data) override;

		void write(std::string const &name, Vec4 const &data) override;
		void write(std::string const &name, std::vector<Vec4> const &data) override;

		void write(std::string const &name, Mat4 const &data) override;
		void write(std::string const &name, std::vector<Mat4> const &data) override;

		template<typename T>
		T const &read(std::string const &name) {
			auto it = uniformsNamesToIndex.find(name);
			if(it == uniformsNamesToIndex.end()) {
				return *reinterpret_cast<T const*>(voidMaterialMemory.data());
			};

			return *reinterpret_cast<T const*>(uniformData.data() + uniformDescriptors[it->second].byteOffset);
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

		SmallVector<UniformInfos, 16> const &getUniformInfos() override;

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
		MaterialInstance();
		MaterialInstance(Ref<Material> mat);

		void write(std::string const &name, int data) override;
		void write(std::string const &name, std::vector<int> const &data) override;

		void write(std::string const &name, unsigned int data) override;
		void write(std::string const &name, std::vector<unsigned int> const &data) override;

		void write(std::string const &name, float data) override;
		void write(std::string const &name, std::vector<float> const &data) override;

		void write(std::string const &name, Vec2 const &data) override;
		void write(std::string const &name, std::vector<Vec2> const &data) override;

		void write(std::string const &name, Vec3 const &data) override;
		void write(std::string const &name, std::vector<Vec3> const &data) override;

		void write(std::string const &name, Vec4 const &data) override;
		void write(std::string const &name, std::vector<Vec4> const &data) override;

		void write(std::string const &name, Mat4 const &data) override;
		void write(std::string const &name, std::vector<Mat4> const &data) override;

		template<typename T>
		T const &read(std::string const &name) {
			auto const &uniformsNamesToIndex = material->uniformsNamesToIndex;

			auto it = uniformsNamesToIndex.find(name);
			if(it == uniformsNamesToIndex.end()) {
				return *reinterpret_cast<T const*>(voidMaterialMemory.data());
			};

			return *reinterpret_cast<T const*>(
				uniformData.data() + material->uniformDescriptors[it->second].byteOffset
			);
		}

		void affect(std::string const &name, Ref<TextureBuffer> texRef);

		void send();

		SmallVector<UniformInfos, 16> const &getUniformInfos() override;

	private:
		Ref<Material> material;

		SmallVector<bool, 16> overridedUniforms;
		SmallVector<bool, 32> overridedTextures;
	};
}
