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
public:
  std::array<std::array<float, 4>, 4> m;
  Matrix4();
  explicit Matrix4(float m_[4][4]);
  explicit Matrix4(std::array<std::array<float, 4>, 4> m_);
  Matrix4(float diag); // diag elements
  Matrix4(const Matrix4 &other);
  Matrix4 &operator=(const Matrix4 &other);
  bool operator==(const Matrix4 &other) const;
  bool operator!=(const Matrix4 &other) const;
  Matrix4 operator*(const Matrix4 &other) const {
    return this->multiply(other);
  }
  bool is_identity() const {
    return almost_equal(m[0][0], 1.0f) && almost_equal(m[1][1], 1.0f) &&
           almost_equal(m[2][2], 1.0f) && almost_equal(m[3][3], 1.0f) &&
           almost_equal(m[0][1], 0.0f) && almost_equal(m[0][2], 0.0f) &&
           almost_equal(m[0][3], 0.0f) && almost_equal(m[1][0], 0.0f) &&
           almost_equal(m[1][2], 0.0f) && almost_equal(m[1][3], 0.0f) &&
           almost_equal(m[2][0], 0.0f) && almost_equal(m[2][1], 0.0f) &&
           almost_equal(m[2][3], 0.0f) && almost_equal(m[3][0], 0.0f) &&
           almost_equal(m[3][1], 0.0f) && almost_equal(m[3][2], 0.0f);
  }
  Matrix4(float row00, float row01, float row02, float row03, float row10,
          float row11, float row12, float row13, float row20, float row21,
          float row22, float row23, float row30, float row31, float row32,
          float row33);
  Matrix4 inverse() const { return Matrix4::Inverse(*this); }
  Matrix4 transpose() const { return Matrix4::Transpose(*this); }
  Matrix4 multiply(const Matrix4 &other) const {
    return Matrix4::Multiply(*this, other);
  }
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
  //  return this->m == other.m;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (!almost_equal(m[i][j], other.m[i][j]))
        return false;
    }
  }
  return true;
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

inline Matrix4 Matrix4::Inverse(const Matrix4 &mat1) {
  // from scrachPixel
  auto mat = Matrix4();
  int N = 4;
  auto copy = mat1;
  auto &m = copy.m;
  for (unsigned column = 0; column < N; ++column) {
    // Swap row in case our pivot point is not working
    if (m[column][column] == 0) {
      unsigned big = column;
      for (unsigned row = 0; row < N; ++row)
        if (fabs(m[row][column]) > fabs(m[big][column]))
          big = row;
      // Print this is a singular matrix, return identity ?
      if (big == column)
        throw std::runtime_error("Matrix4 is not invertible");
      // Swap rows
      else
        for (unsigned j = 0; j < N; ++j) {
          std::swap(m[column][j], m[big][j]);
          std::swap(mat.m[column][j], mat.m[big][j]);
        }
    }
    // Set each row in the column to 0
    for (unsigned row = 0; row < N; ++row) {
      if (row != column) {
        float coeff = m[row][column] / m[column][column];
        if (coeff != 0) {
          for (unsigned j = 0; j < N; ++j) {
            m[row][j] -= coeff * m[column][j];
            mat.m[row][j] -= coeff * mat.m[column][j];
          }
          // Set the element to 0 for safety
          m[row][column] = 0;
        }
      }
    }
  }
  // Set each element of the diagonal to 1
  for (unsigned row = 0; row < N; ++row) {
    for (unsigned column = 0; column < N; ++column) {
      mat.m[row][column] /= m[row][row];
    }
  }
  return mat;
}

inline Matrix4::Matrix4(std::array<std::array<float, 4>, 4> m_) {
  this->m = m_;
}

inline Matrix4 Matrix4::Transpose(const Matrix4 &mat) {
  Matrix4 copy = mat;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) {
      copy.m[i][j] = mat.m[j][i];
    }
  return copy;
}

inline Matrix4 Matrix4::Multiply(const Matrix4 &lhs, const Matrix4 &rhs) {
  Matrix4 result;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      result.m[i][j] = lhs.m[i][0] * rhs.m[0][j] + lhs.m[i][1] * rhs.m[1][j] +
                       lhs.m[i][2] * rhs.m[2][j] + lhs.m[i][3] * rhs.m[3][j];
    }
  }
  return result;
}

NAMESPACE_END(DR)
