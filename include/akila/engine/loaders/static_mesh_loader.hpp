#pragma once

#include "akila/core/resources/loader.hpp"

namespace akila {
	class StaticMeshLoader: public Loader {
	public:
		StaticMeshLoader();
		void onEntry(JSON json, LoaderCallback cb) override;
	};
}
