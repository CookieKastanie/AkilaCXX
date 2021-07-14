#pragma once

#include "Akila/graphics/gl/Shader.hpp"
#include <memory>
#include <vector>

namespace Akila {
	class Material {
	private:
		static int lastId;

		int id;

		std::shared_ptr<Shader> shader;

		union UniformValueType {
			float f;
			int i;
		};
		struct UniformValue {
			unsigned int uid;
			std::vector<UniformValueType> values;
		};

		std::vector<UniformValue> uniformsFloat;
		std::vector<UniformValue> uniformsInts;

	public:
		static int currentUsed;

		Material();

		int getId() const;

		Shader *getShader() const;
		void setShader(const std::shared_ptr<Shader> &shader);

		void addUniformValue(const UniformValue &uv, bool isInts = false);
		void sendUniforms() const;
	};
}
