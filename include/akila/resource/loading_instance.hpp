#pragma once

#include <string>
#include <functional>
#include <initializer_list>

namespace akila {
	class Resources;
	class LoaderCallback;
}

namespace akila::internal {
	class LoadingInstance {
	private:
		friend class Resources;
		friend class LoaderCallback;

		std::function<void()> finishedCallback;
		std::vector<std::string> paths;
		std::size_t count;

		LoadingInstance(std::string const &path, std::function<void()> const &callback);
		LoadingInstance(std::initializer_list<std::string> const &paths, std::function<void()> const &callback);

		void start();
		void countDown();
	};
}
