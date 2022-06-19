#pragma once

#include "akila/memory/ref.hpp"
#include "akila/renderer/shader.hpp"

namespace akila {
	class Material {
	public:
		Material(Ref<Shader> shader);
		//void set(std::string uniformName);
		void send();

	private:
		unsigned int id;
		Ref<Shader> shader;
	};
}
