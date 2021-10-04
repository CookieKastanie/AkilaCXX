#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

#include "Akila/core/Time.hpp"

namespace Akila {
	class Transform {
    private:
        glm::vec3 prevPosition;
        glm::quat prevRotation;

        glm::vec3 position;
        glm::quat rotation;

        glm::mat4 matrix;

    public:
        Transform();

        void translate(float dx, float dy, float dz);
        void setPosition(float x, float y, float z);

        glm::vec3 &getPosition();

        void rotateX(float a);
        void rotateY(float a);
        void rotateZ(float a);

        void setRotationZYX(float ax, float ay, float az);
        glm::quat &getRotation();
        glm::mat4 &toMatrix();
        glm::mat4 &toMatrixFromOrigin(const glm::mat4 &o);

        void savePrevious();
        glm::mat4 &toMatrixMix(const float t = Time::mix);
	};
}
