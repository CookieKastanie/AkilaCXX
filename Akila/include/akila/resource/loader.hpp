#pragma once

#include <string>
#include "akila/resource/json.hpp"

namespace akila {
	namespace internal {
		class LoadingInstance;
	}

	class LoaderCallback {
	public:
		void success();
		void fail();

	private:
		friend class internal::LoadingInstance;

		internal::LoadingInstance *li;
		bool used;

		LoaderCallback(internal::LoadingInstance *li);
	};

	class Loader {
	public:
		Loader(std::string const &listName);
		virtual ~Loader() = default;

		virtual void onEntry(JSON json, LoaderCallback cb) = 0;
		std::string const &getListName() const;

	private:
		std::string listName;
	};
}
