#pragma once

#include "akila/core/math/math.hpp"

namespace akila {
	class CameraData {
	public:
		CameraData();

		Mat4 const &getProjection();
		Mat4 const &getView();
		Mat4 const &getPV();
		Vec3 const &getPosition();
		float getRatio();

		CameraData const *getData();

	protected:
		Mat4 projection;
		Mat4 view;
		Mat4 pv;
		Vec3 position;
		float ratio;
	};

	class Camera: public CameraData {
	public:
		enum class ProjectionType {
			PERSPRECTIVE,
			ORTHOGRAPHIC
		};

		Camera(ProjectionType type = ProjectionType::PERSPRECTIVE);
		
		ProjectionType getProjectionType();
		void resize(IVec2 const &size);
		

	protected:
		ProjectionType type;
		float near;
		float far;
		float fov;
		//IVec4 viewport; // left, right, up, down

		void refreshProjection();
	};
}
