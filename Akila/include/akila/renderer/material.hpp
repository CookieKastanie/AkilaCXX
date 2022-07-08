#pragma once

#include "akila/memory/ref.hpp"
#include "akila/renderer/shader.hpp"
#include "akila/math/math.hpp"

namespace akila {
	class Material {
	public:
		Material() = default;
		Material(std::string const &shader);

		void render() {
			shader.bind();
			glBindVertexArray(vb);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0);
		}

	//private:
		friend class MaterialInstance;

		Shader shader;

		GLuint vb;

		struct UniformDataRef {
			unsigned int shaderLocation;
			UniformUnderlyingType baseType;

			std::size_t offset;
			int size; // vec1 vec2 vec3
			int count;
		};

		std::unordered_map<std::string, UniformDataRef> uniforms;
	};

	class MaterialInstance {
	public:
		MaterialInstance(Ref<Material> material);

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

	private:
		Ref<Material> material;
		
		std::vector<unsigned char> uniformData;

		template<typename T>
		inline void writeRaw(std::size_t offset, T *data, std::size_t count) {
			std::memcpy(uniformData.data() + offset, data, count * sizeof(T));
		}
	};
}
