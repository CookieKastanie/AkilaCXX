#pragma once

#include "akila/core/resources/loader.hpp"

namespace akila {
	class Texture2DLoader: public Loader {
	public:
		Texture2DLoader();
		void onEntry(JSON json, LoaderCallback cb) override;
	};
}
