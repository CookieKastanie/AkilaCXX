#pragma once

#pragma warning(push, 0)
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#pragma warning(pop)

#include <iostream>

#define NOMINMAX
#undef min
#undef max

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
	using glm::min;
	using glm::max;
	using glm::clamp;
	using glm::lerp;
	using glm::slerp;
	using glm::toMat4;
	using glm::translate;
	using glm::scale;
	using glm::angleAxis;
	using glm::cross;
	using glm::mix;
	using glm::radians;

	using glm::perspective;
	using glm::ortho;
	using glm::lookAt;

	using glm::to_string;


	constexpr float const PI = 3.14159265359f;
	constexpr float const HALF_PI = 1.57079632679f;
	constexpr float const NATURAL_LOG_OF_2 = 0.69314718056f;
	constexpr float const GOLDEN_RATIO = 1.61803398875f;

	inline float toRadians(float degree) { return degree * (PI / 180.f); }
}

std::ostream &operator<<(std::ostream &os, akila::Vec1 const &v);
std::ostream &operator<<(std::ostream &os, akila::IVec1 const &v);

std::ostream &operator<<(std::ostream &os, akila::Vec2 const &v);
std::ostream &operator<<(std::ostream &os, akila::IVec2 const &v);

std::ostream &operator<<(std::ostream &os, akila::Vec3 const &v);
std::ostream &operator<<(std::ostream &os, akila::IVec3 const &v);

std::ostream &operator<<(std::ostream &os, akila::Vec4 const &v);
std::ostream &operator<<(std::ostream &os, akila::IVec4 const &v);

std::ostream &operator<<(std::ostream &os, akila::Mat2 const &v);
std::ostream &operator<<(std::ostream &os, akila::IMat2 const &v);

std::ostream &operator<<(std::ostream &os, akila::Mat3 const &v);
std::ostream &operator<<(std::ostream &os, akila::IMat3 const &v);

std::ostream &operator<<(std::ostream &os, akila::Mat4 const &v);
std::ostream &operator<<(std::ostream &os, akila::IMat4 const &v);

std::ostream &operator<<(std::ostream &os, akila::Quat const &v);
