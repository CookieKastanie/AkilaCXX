#pragma once

namespace Akila {
	class State {
		public:
			State();
			virtual ~State();

			virtual void update() = 0;
			virtual void draw() = 0;
	};
}