#pragma once

#include "akila/core/math/math.hpp"

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

		struct Bounds {
			Vec3 min = {0.f, 0.f, 0.f};
			Vec3 max = {0.f, 0.f, 0.f};
			float radius = 0.f;
			float squaredRadius = 0.f;
		};

		Mesh();

		virtual void draw() const = 0;
		void setBounds(Bounds const &bounds);
		Bounds const &getBounds();

	protected:
		Bounds bounds;
	};
}
