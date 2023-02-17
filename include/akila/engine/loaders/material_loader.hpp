#pragma once

#include "akila/core/resources/loader.hpp"

namespace akila {
	class MaterialLoader: public Loader {
	public:
		MaterialLoader();
		void onEntry(JSON json, LoaderCallback cb) override;
	};
}
