#pragma once

#include "akila/core/common/type_infos.hpp"

namespace akila {
	namespace internal {
		class ISignalQueue;
	}

	using ListenerId = std::size_t;

	class Listener {
	public:
		Listener();

		Listener(Listener const &) = delete;
		Listener &operator=(Listener const &) = delete;

		Listener(Listener &&l) noexcept;
		Listener &operator=(Listener &&l) noexcept;

		~Listener();

	private:
		friend class Signals;
		friend class internal::ISignalQueue;

		TypeId type;
		ListenerId id;
		
		Listener(TypeId type, ListenerId id);
	};
}
