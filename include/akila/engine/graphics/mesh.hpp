#pragma once

namespace akila {
	class Mesh {
	public:
		enum Attributes: unsigned int {
			POSITION = 0,
			NORMAL = 1,
			TEXCOORD = 2,
			TANGENT = 3,
			COLOR = 4,
			JOINTS = 5,
			WEIGHTS = 6,

			TEXCOORD_0 = TEXCOORD,
			TEXCOORD_1 = 7,
			TEXCOORD_2 = 8,

			COLOR_0 = COLOR,
			COLOR_1 = 9,
			COLOR_2 = 10,

			JOINTS_0 = JOINTS,
			JOINTS_1 = 11,
			JOINTS_2 = 12,

			WEIGHTS_0 = WEIGHTS,
			WEIGHTS_1 = 13,
			WEIGHTS_2 = 14,

			ANY = 15
		};

		virtual void draw() const = 0;
	};
}
