#include "akila/math/math.hpp"
#include <glm/gtx/string_cast.hpp>

using namespace akila;

#define PRINT_IMPL {os << glm::to_string(v); return os;}

std::ostream &operator<<(std::ostream &os, Vec1 const &v) PRINT_IMPL
std::ostream &operator<<(std::ostream &os, IVec1 const &v) PRINT_IMPL

std::ostream &operator<<(std::ostream &os, Vec2 const &v) PRINT_IMPL
std::ostream &operator<<(std::ostream &os, IVec2 const &v) PRINT_IMPL

std::ostream &operator<<(std::ostream &os, Vec3 const &v) PRINT_IMPL
std::ostream &operator<<(std::ostream &os, IVec3 const &v) PRINT_IMPL

std::ostream &operator<<(std::ostream &os, Vec4 const &v) PRINT_IMPL
std::ostream &operator<<(std::ostream &os, IVec4 const &v) PRINT_IMPL

std::ostream &operator<<(std::ostream &os, Mat2 const &v) PRINT_IMPL
std::ostream &operator<<(std::ostream &os, IMat2 const &v) PRINT_IMPL

std::ostream &operator<<(std::ostream &os, Mat3 const &v) PRINT_IMPL
std::ostream &operator<<(std::ostream &os, IMat3 const &v) PRINT_IMPL

std::ostream &operator<<(std::ostream &os, Mat4 const &v) PRINT_IMPL
std::ostream &operator<<(std::ostream &os, IMat4 const &v) PRINT_IMPL

std::ostream &operator<<(std::ostream &os, Quat const &v) PRINT_IMPL
