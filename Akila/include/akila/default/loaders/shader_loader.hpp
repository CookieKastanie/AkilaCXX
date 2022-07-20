#pragma once

#include "akila/resource/loader.hpp"

namespace akila {
	class ShaderLoader: public Loader {
	public:
		ShaderLoader();
		void onEntry(JSON json, LoaderCallback cb) override;
	};
}
