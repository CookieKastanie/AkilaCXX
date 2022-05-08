#pragma once

#include "akila/common/type_infos.hpp"

namespace akila {
	class Listener {
	public:
		~Listener();

	private:
		friend class Events;

		TypeId type;
		std::size_t id;
		
		Listener(TypeId type, std::size_t id);
	};
}
