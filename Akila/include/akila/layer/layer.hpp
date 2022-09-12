#pragma once

#include "akila/common/type_infos.hpp"

namespace akila {
	class Layer {
	public:
		enum class Depth: char {
			BACK = -1,
			MIDDLE = 0,
			FRONT = 1
		};

		Layer();
		virtual ~Layer() = default;

		virtual void tick();
		virtual void frame();
		virtual void gui();

		std::string const getTypeName() const;

	private:
		friend class Layers;

		TypeId typeId;
		Depth depth;

		std::string typeName;
	};
}
