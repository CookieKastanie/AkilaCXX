#pragma once

#include "akila/core/common/type_infos.hpp"

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

		Layer(Layer const &) = default;
		virtual Layer &operator=(Layer const &) = default;
		Layer(Layer &&) = default;
		virtual Layer &operator=(Layer &&) = default;

		virtual void onMount();
		virtual void onUnmount();

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
