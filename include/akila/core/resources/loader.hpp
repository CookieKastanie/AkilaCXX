#pragma once

#include <string>
#include "akila/core/resources/json.hpp"

namespace akila {
	namespace internal {
		class LoadingInstance;
	}

	class LoaderCallback {
	public:
		void success() const;
		void fail() const;

	private:
		friend class internal::LoadingInstance;

		internal::LoadingInstance *li;
		mutable bool used;

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
