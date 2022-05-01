#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace akila {
	using Vec1 = glm::vec1;
	using IVec1 = glm::ivec1;

	using Vec2 = glm::vec2;
	using IVec2 = glm::ivec2;

	using Vec3 = glm::vec3;
	using IVec3 = glm::ivec3;

	using Vec4 = glm::vec4;
	using IVec4 = glm::ivec4;

	using Mat2 = glm::mat2x2;
	using IMat2 = glm::imat2x2;

	using Mat3 = glm::mat3x3;
	using IMat3 = glm::imat3x3;

	using Mat4 = glm::mat4x4;
	using IMat4 = glm::imat4x4;

	using Quat = glm::quat;


	using glm::cos;
	using glm::sin;
	using glm::tan;
	using glm::acos;
	using glm::asin;
	using glm::atan;

	using glm::abs;
	using glm::clamp;
	using glm::lerp;
	using glm::slerp;
	using glm::toMat4;
	using glm::translate;
	using glm::scale;
	using glm::angleAxis;
	using glm::cross;
}