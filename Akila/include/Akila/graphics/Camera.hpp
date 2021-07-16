#pragma once 

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Akila {
	class Camera {
		protected:
			struct Uniforms {
				glm::mat4 projection;
				glm::mat4 view;
				glm::mat4 pv;
				glm::vec3 position;
				float ratio;
			};

			Uniforms uniforms;

		public:
			Camera();
			virtual ~Camera();

			virtual void onResize(int width, int height);
			virtual void update();

			void setPosition(const glm::vec3 &pos);
			glm::vec3 &getPosition();

			Uniforms &getUniforms();
	};
}
