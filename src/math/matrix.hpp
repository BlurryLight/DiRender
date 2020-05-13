#pragma once
#include <array>
#include <iostream>
#include <math/math_utils.hpp>
#include <math/vector.hpp>
#include <utils/di_global.h>
NAMESPACE_BEGIN(DR)

struct Matrix4;
using mat4 = struct Matrix4;
struct Matrix4 {
  std::array<std::array<float, 4>, 4> m;
  Matrix4();
  explicit Matrix4(float m_[4][4]);
  explicit Matrix4(std::array<std::array<float, 4>, 4> m_);
  Matrix4(float diag); // diag elements
  Matrix4(const Matrix4 &other);
  Matrix4 &operator=(const Matrix4 &other);
  bool operator==(const Matrix4 &other) const;
  bool operator!=(const Matrix4 &other) const;
  Matrix4(float row00, float row01, float row02, float row03, float row10,
          float row11, float row12, float row13, float row20, float row21,
          float row22, float row23, float row30, float row31, float row32,
          float row33);
  Matrix4 inverse() const { return Matrix4::Inverse(*this); }
  Matrix4 transpose() const;
  Matrix4 multiply() const;
  const float *data() const;
  friend std::ostream &operator<<(std::ostream &os, const Matrix4 &mat4) {
    for (const auto &row : mat4.m) {
      os << "[ ";
      for (const auto &elem : row) {
        os << elem << " ";
      }
      os << "]" << std::endl;
    }
    return os;
  }
  static Matrix4 Inverse(const Matrix4 &mat);
  static Matrix4 Transpose(const Matrix4 &mat);
  static Matrix4 Multiply(const Matrix4 &lhs, const Matrix4 &rhs);
};

inline Matrix4::Matrix4(float diag) {
  for (auto &i : m)
    i.fill(0.0f);
  for (int i = 0; i < 4; i++)
    m[i][i] = diag;
}
inline Matrix4::Matrix4() : Matrix4(1.0f) {}

inline const float *Matrix4::data() const { return m[0].data(); }

inline Matrix4::Matrix4(const Matrix4 &other) { this->m = other.m; }

inline Matrix4::Matrix4(float m_[4][4]) {
  std::copy_n(&m_[0][0], 16, this->m[0].begin());
}

inline Matrix4 &Matrix4::operator=(const Matrix4 &other) {
  this->m = other.m;
  return (*this);
}

inline bool Matrix4::operator==(const Matrix4 &other) const {
  return this->m == other.m;
}

inline bool Matrix4::operator!=(const Matrix4 &other) const {
  return !(this->m == other.m);
}

inline Matrix4::Matrix4(float row00, float row01, float row02, float row03,
                        float row10, float row11, float row12, float row13,
                        float row20, float row21, float row22, float row23,
                        float row30, float row31, float row32, float row33) {
  m[0][0] = row00;
  m[0][1] = row01;
  m[0][2] = row02;
  m[0][3] = row03;

  m[1][0] = row10;
  m[1][1] = row11;
  m[1][2] = row12;
  m[1][3] = row13;

  m[2][0] = row20;
  m[2][1] = row21;
  m[2][2] = row22;
  m[2][3] = row23;

  m[3][0] = row30;
  m[3][1] = row31;
  m[3][2] = row32;
  m[3][3] = row33;
}

inline Matrix4 Matrix4::Inverse(const Matrix4 &mat) {
  auto copy = mat.m;
  auto result = Matrix4();
  int row = 0, col = 0;
  while (row < 4 && col < 4) {
    // argmax
    float row_max_value = kFloatLowest;
    int row_max = 0;
    for (int i = row; i < 4; i++) {
      if (row_max_value < std::abs(copy[i][col])) {
        row_max_value = copy[i][col];
        row_max = i;
      }
    }

    if (almost_equal(row_max_value, 0.0f)) {
      col += 1; // no pivot in this column
    } else {
      for (int i = 0; i < 4; i++) {
        std::swap(copy[row_max][i], copy[row][i]);
        std::swap(result.m[row_max][i], result.m[row][i]);
      }
      for (int i = row + 1; i < 4; i++) {
        float f = copy[i][col] / copy[row][col];
        copy[i][col] = 0;
        for (int j = col + 1; j < 4; j++) {
          copy[i][j] = copy[i][j] - copy[row][j] * f;
          result.m[i][j] = result.m[i][j] - result.m[row][j] * f;
        }
      }
      row += 1;
      col += 1;
    }
  }

  for (int i = 0; i < 4; i++) {
    float f = copy[i][i];
    if (almost_equal(f, 0.0f)) {
      throw std::runtime_error("Matrix4 is not invertible");
    }
    for (int j = i; j < 4; j++) {
      copy[i][j] /= f;
      result.m[i][j] /= f;
    }
    for (int k = i + 1; k < 4; k++) {
      f = copy[k][i];
      for (int j = 0; j < 4; j++) {
        copy[j][k] -= f * copy[j][i];
        result.m[j][k] -= f * result.m[j][i];
      }
    }
  }
  return result;
}

inline Matrix4::Matrix4(std::array<std::array<float, 4>, 4> m_) {
  this->m = m_;
}

NAMESPACE_END(DR)
