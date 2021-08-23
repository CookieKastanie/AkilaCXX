#pragma once

#include "Akila/core/ResourcePool.hpp"
#include "Akila/core/Task.hpp"
#include <functional>

namespace Akila {
	class ResourceFileParser {
	private:
		friend class ResourcePool;

		class LoadingInstance {
		public:
			bool fileEnd;
			int fileCount;
			int loadedFileCount;

			std::function<void()> callback;

			LoadingInstance();

			void check();
		};

		ResourcePool *rp;
		TaskManager *taskManager;
		const std::function<void()> &onFinish;
		std::shared_ptr<LoadingInstance> loadingInstance;

		ResourceFileParser(ResourcePool *rp, const std::function<void()> &onFinish, TaskManager *taskManager);



		struct ShaderState {
			static const int STATE_ID = 0;

			std::string name;
			std::string src;

			// initial values
			std::vector<std::string> unifNames;
			std::vector<bool> unifIsInt;
			std::vector<Material::UniformValue> uniformsValues;

			void clear();
		};

		struct TextureState {
			static const int STATE_ID = 1;

			std::string name;
			std::string src;
			TextureBuffer::Format format;
			TextureBuffer::Parameters parameters;
			bool generateMips;

			void clear();

			static TextureBuffer::Format stringToFormat(const std::string &str);
			static TextureBuffer::WrapMode stringToWrapMode(const std::string &str);
			static TextureBuffer::FilterMode stringToFilterMode(const std::string &str);
			static bool stringToBool(const std::string &str);
		};

		struct MeshState {
			static const int STATE_ID = 2;

			std::string name;
			std::string src;

			void clear();
		};

		struct MaterialState {
			static const int STATE_ID = 3;

			std::string name;
			std::string shader;

			std::vector<std::string> unifNames;
			std::vector<bool> unifIsInt;
			std::vector<Material::UniformValue> uniformsValues;

			std::vector<Material::TextureBinding> textureBindings;

			void clear();
		};

		void createShader(const ShaderState &shaderState);
		void createMaterial(MaterialState &materialState);
		void createTexture(const TextureState &textureState);
		void createMesh(const MeshState &meshState);
		void fromFile(const std::string &path);
	};
}
