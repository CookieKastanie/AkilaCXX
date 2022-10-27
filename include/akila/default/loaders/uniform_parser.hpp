#pragma once

#include "akila/akila.hpp"

namespace akila::Parser {
	void parseUniforms(Shader *shader, JSON &json, std::function<void(std::string const &name, UniformInfos const &infos, void *data, std::size_t byteCount)> onData);

	void parseTextureBinds(Shader *shader, JSON &json, std::function<void(Ref<TextureBuffer> texRef, int unit)> onInt, std::function<void(Ref<TextureBuffer> texRef, std::string const &unifName)> onStr);
}
