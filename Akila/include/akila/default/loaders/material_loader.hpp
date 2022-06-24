#pragma once

#include "akila/resource/loader.hpp"

namespace akila {
	class MaterialLoader: public Loader {
	public:
		MaterialLoader();
		void onEntry(JSON json, LoaderCallback cb) override;
	};
}
