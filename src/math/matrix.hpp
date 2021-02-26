#pragma once
#include <array>
#include <functional>
#include <iostream>
#include <math/math_utils.hpp>
#include <math/vector.hpp>
#include <utils/di_global.h>
#ifdef DI_USE_GLM
#include <glm/glm.hpp>
#endif

NAMESPACE_BEGIN(DR)

struct Matrix4;
using mat4 = struct Matrix4;
struct Matrix4 {
public:
#ifndef DI_USE_GLM
  std::array<std::array<float, 4>, 4> m{};
#else
  glm::mat4 m;
  Matrix4(const glm::mat4 &m_);
#endif
  Matrix4();
  explicit Matrix4(float m_[4][4]);
  explicit Matrix4(std::array<std::array<float, 4>, 4> m_);
  Matrix4(float diag); // diag elements
  Matrix4(const Matrix4 &other);
  Matrix4 &operator=(const Matrix4 &other);
  bool operator==(const Matrix4 &other) const;
  bool operator!=(const Matrix4 &other) const;
  auto &operator[](int index) { return m[index]; }
  auto operator[](int index) const { return m[index]; }
  Matrix4 operator*(const Matrix4 &other) const {
    return this->multiply(other);
  }
  bool is_identity() const;
  Matrix4(float row00, float row01, float row02, float row03, float row10,
          float row11, float row12, float row13, float row20, float row21,
          float row22, float row23, float row30, float row31, float row32,
          float row33);
  Matrix4 inverse() const { return Matrix4::Inverse(*this); }
  Matrix4 transpose() const { return Matrix4::Transpose(*this); }
  Matrix4 multiply(const Matrix4 &other) const {
    return Matrix4::Multiply(*this, other);
  }
  //  const float *data() const;
  static Matrix4 Inverse(const Matrix4 &mat);
  static Matrix4 Transpose(const Matrix4 &mat);
  static Matrix4 Multiply(const Matrix4 &lhs, const Matrix4 &rhs);

  static Matrix4 Translate(Vector3f vec);
  static Matrix4 Translate(const Matrix4 &m, Vector3f vec);
  static Matrix4 Rotate(float rad, Vector3f axis);
  static Matrix4 Rotate(const Matrix4 &m, float rad, Vector3f vec);
  static Matrix4 Scale(Vector3f vec);
  static Matrix4 Scale(const Matrix4 &m, Vector3f vec);

  friend std::ostream &operator<<(std::ostream &os, const Matrix4 &mat4);
};
std::ostream &operator<<(std::ostream &os, const Matrix4 &mat4);
NAMESPACE_END(DR)

namespace std {
template <> class hash<DR::Matrix4> {
public:
  size_t operator()(const DR::Matrix4 &mat);
};
} // namespace std
