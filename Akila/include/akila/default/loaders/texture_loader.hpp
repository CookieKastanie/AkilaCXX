#pragma once

#include "akila/resource/loader.hpp"
#include "akila/akila.hpp"

namespace akila {
	class Texture2DLoader: public Loader {
	public:
		Texture2DLoader();
		void onEntry(JSON json, LoaderCallback cb) override;

		static void write(std::string const &path, akila::TextureBuffer *texture, bool invertY = true);
	};
}
