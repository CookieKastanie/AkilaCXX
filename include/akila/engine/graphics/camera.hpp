#pragma once

#include "akila/core/math/math.hpp"

namespace akila {
	struct CameraData {
	public:
		CameraData();

		Mat4 projection;
		Mat4 view;
		Mat4 pv;
		Vec3 position;
		float ratio;

		CameraData const *getData();
	};

	struct Camera: public CameraData {
	public:
		enum class ProjectionType {
			PERSPRECTIVE,
			ORTHOGRAPHIC
		};

		Camera(ProjectionType type = ProjectionType::PERSPRECTIVE);
		
		ProjectionType type;
		float near;
		float far;
		float fov;
		//IVec4 viewport; // left, right, up, down

		void refreshProjection();
		void resize(IVec2 const &size);
	};
}
