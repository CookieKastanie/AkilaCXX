#pragma once

#include "akila/core/resources/loader.hpp"

namespace akila {
	class AudioLoader: public Loader {
	public:
		AudioLoader();
		void onEntry(JSON json, LoaderCallback cb) override;
	};
}
