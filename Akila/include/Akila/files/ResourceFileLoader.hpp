#pragma once

#include "Akila/core/ResourcePool.hpp"
#include "nlohmann/json.hpp"
#include <string>
#include <functional>

namespace Akila {
	class ResourceFileLoader {
	private:
		struct LoadingInstance {
		private:
			int count;

		public:
			LoadingInstance(std::function<void()> const &callback);

			std::function<void()> callback;
			void countUp();
			void countDown();
			bool isFinished();
		};

	public:
		static void fillResourcePool(ResourcePool *rp, nlohmann::json &file, std::function<void()> const &callback);
		static void fillResourcePool(ResourcePool *rp, std::string const &path, std::function<void()> const &callback);
	};
}
