#pragma once

#include "akila/resource/loader.hpp"

namespace akila {
	class Texture2DLoader: public Loader {
	public:
		Texture2DLoader();
		void onEntry(JSON json, LoaderCallback cb) override;
	};
}
