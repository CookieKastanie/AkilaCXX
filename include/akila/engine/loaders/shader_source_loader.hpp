#pragma once

#include "akila/core/resources/loader.hpp"

namespace akila {
	class ShaderSourceLoader: public Loader {
	public:
		ShaderSourceLoader();
		void onEntry(JSON json, LoaderCallback cb) override;
	};
}
