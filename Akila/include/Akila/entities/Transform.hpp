#pragma once

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

#include "Akila/core/Time.hpp"

namespace Akila {
	class Transform {
    private:
        glm::vec3 prevPosition;
        glm::quat prevRotation;
        glm::vec3 prevScale;

        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;

        glm::mat4 matrix;

    public:
        Transform();

        void translate(float dx, float dy, float dz);
        void setPosition(float x, float y, float z);
        void setPosition(glm::vec3 const &vec);

        glm::vec3 &getPosition();

        void rotateX(float a);
        void rotateY(float a);
        void rotateZ(float a);

        void setRotationZYX(float ax, float ay, float az);
        glm::quat &getRotation();

        void setScale(float x, float y, float z);
        void setScale(float s);

        glm::mat4 &toMatrix();
        glm::mat4 &toMatrixFromOrigin(const glm::mat4 &o);

        void savePrevious();
        glm::mat4 &toMatrixMix(const float t = Time::mix);
	};
}
