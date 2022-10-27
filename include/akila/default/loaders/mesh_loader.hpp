#pragma once

#include "akila/resource/loader.hpp"

namespace akila {
	class MeshLoader: public Loader {
	public:
		MeshLoader();
		void onEntry(JSON json, LoaderCallback cb) override;
	};
}
