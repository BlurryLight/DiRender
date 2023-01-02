//
// Created by panda on 2021/2/18.
//
#include <math/matrix.hpp>
#include <spdlog/fmt/fmt.h>
using namespace DR;
mat4 DR::Matrix4::Identity_ = mat4();
#ifndef DI_USE_GLM
bool DR::Matrix4::is_identity() const { return *this == Identity_; }

Matrix4::Matrix4(float diag) : m() {
  for (auto &i : m)
    i.fill(0.0f);
  for (int i = 0; i < 4; i++)
    m[i][i] = diag;
}
Matrix4::Matrix4() : Matrix4(1.0f) {}

// const float *Matrix4::data() const { return m[0].data(); }

Matrix4::Matrix4(const Matrix4 &other) { this->m = other.m; }

Matrix4::Matrix4(float m_[4][4]) {
  std::copy_n(&m_[0][0], 16, this->m[0].begin());
}

Matrix4 &Matrix4::operator=(const Matrix4 &other) = default;

bool Matrix4::operator==(const Matrix4 &other) const {
  //  return this->m == other.m;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (!abs_almost_equal(m[i][j], other.m[i][j]))
        return false;
    }
  }
  return true;
}

bool Matrix4::operator!=(const Matrix4 &other) const {
  return !(this->m == other.m);
}

Matrix4::Matrix4(float row00, float row01, float row02, float row03,
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

Matrix4 Matrix4::Inverse(const Matrix4 &mat1) {
  // from scrachPixel
  auto mat = Matrix4();
  uint N = 4;
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

Matrix4::Matrix4(std::array<std::array<float, 4>, 4> m_) { this->m = m_; }

Matrix4 Matrix4::Transpose(const Matrix4 &mat) {
  Matrix4 copy = mat;
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) {
      copy.m[i][j] = mat.m[j][i];
    }
  return copy;
}

Matrix4 Matrix4::Multiply(const Matrix4 &lhs, const Matrix4 &rhs) {
  Matrix4 result;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      result.m[i][j] = lhs.m[i][0] * rhs.m[0][j] + lhs.m[i][1] * rhs.m[1][j] +
                       lhs.m[i][2] * rhs.m[2][j] + lhs.m[i][3] * rhs.m[3][j];
    }
  }
  return result;
}
std::ostream &DR::operator<<(std::ostream &os, const Matrix4 &mat4) {
  for (const auto &row : mat4.m) {
    os << "[ ";
    for (const auto &elem : row) {
      os << elem << " ";
    }
    os << "]" << std::endl;
  }
  return os;
}
Matrix4 Matrix4::Rotate(const Matrix4 &m, float rad, Vector3f vec) {
  return m * Matrix4::Rotate(rad, vec);
}

Matrix4 Matrix4::Scale(Vector3f vec) {
  mat4 mat(1.0f);
  mat[0][0] = vec[0];
  mat[1][1] = vec[1];
  mat[2][2] = vec[2];
  return mat;
}
Matrix4 Matrix4::Scale(const Matrix4 &m, Vector3f vec) {
  return m * Matrix4::Scale(vec);
}

// right hand axis
Matrix4 Matrix4::Rotate(float rad, Vector3f axis) {
  // magic here. Goto Lingqi Yan Games101 Lecture 04
  axis = axis.normalize();
  if (rad < -2 * kPi && rad > 2 * kPi)
    throw std::runtime_error("Rotate only accepts rad [-2pi, 2pi]!");
  // special vec
  static vec3 xaxis{1.0f, 0.0f, 0.0f};
  static vec3 yaxis{0.0f, 1.0f, 0.0f};
  static vec3 zaxis{0.0f, 0.0f, 1.0f};
  mat4 mat(1.0f);
  float cos_val = std::cos(rad);
  float sin_val = std::sin(rad);
  if (axis == xaxis || axis == yaxis || axis == zaxis) {
    if (axis == xaxis) {
      mat[1][1] = cos_val;
      mat[1][2] = -sin_val;
      mat[2][1] = sin_val;
      mat[2][2] = cos_val;
      return mat;
    } else if (axis == yaxis) {
      mat[0][0] = cos_val;
      mat[0][2] = sin_val;
      mat[2][0] = -sin_val;
      mat[2][2] = cos_val;
      return mat;
    }
    // zaxis
    else {
      mat[0][0] = cos_val;
      mat[0][1] = -sin_val;
      mat[1][0] = sin_val;
      mat[1][1] = cos_val;
      return mat;
    }
  }
  // arbitrary axis
  // http://scipp.ucsc.edu/~haber/ph216/rotation_12.pdf
  mat4 magic(1.0);
  magic[0][0] = axis[0] * axis[0] * (1 - cos_val) + cos_val;
  magic[0][1] = axis[0] * axis[1] * (1 - cos_val) - axis[2] * sin_val;
  magic[0][2] = axis[0] * axis[2] * (1 - cos_val) + axis[1] * sin_val;

  magic[1][0] = axis[0] * axis[1] * (1 - cos_val) + axis[2] * sin_val;
  magic[1][1] = axis[1] * axis[1] * (1 - cos_val) + cos_val;
  magic[1][2] = axis[1] * axis[2] * (1 - cos_val) - axis[0] * sin_val;

  magic[2][0] = axis[0] * axis[2] * (1 - cos_val) - axis[1] * sin_val;
  magic[2][1] = axis[1] * axis[2] * (1 - cos_val) + axis[0] * sin_val;
  magic[2][2] = axis[2] * axis[2] * (1 - cos_val) + cos_val;
  return magic;
}

Matrix4 Matrix4::Translate(Vector3f vec) {
  Matrix4 mat(1.0f);
  mat.m[0][3] = vec[0];
  mat.m[1][3] = vec[1];
  mat.m[2][3] = vec[2];
  return mat;
}
Matrix4 Matrix4::Translate(const Matrix4 &m, Vector3f vec) {
  return m * Matrix4::Translate(vec);
}

#else

bool DR::Matrix4::is_identity() const {
  return almost_equal(m[0][0], 1.0f) && almost_equal(m[1][1], 1.0f) &&
         almost_equal(m[2][2], 1.0f) && almost_equal(m[3][3], 1.0f) &&
         almost_equal(m[0][1], 0.0f) && almost_equal(m[0][2], 0.0f) &&
         almost_equal(m[0][3], 0.0f) && almost_equal(m[1][0], 0.0f) &&
         almost_equal(m[1][2], 0.0f) && almost_equal(m[1][3], 0.0f) &&
         almost_equal(m[2][0], 0.0f) && almost_equal(m[2][1], 0.0f) &&
         almost_equal(m[2][3], 0.0f) && almost_equal(m[3][0], 0.0f) &&
         almost_equal(m[3][1], 0.0f) && almost_equal(m[3][2], 0.0f);
}

Matrix4::Matrix4(float diag) : m(diag) {}

Matrix4::Matrix4(const glm::mat4 &m_) : m(m_) {}
Matrix4::Matrix4() : Matrix4(1.0f) {}

// const float *Matrix4::data() const { return m[0].data(); }

Matrix4::Matrix4(const Matrix4 &other) { this->m = other.m; }

Matrix4::Matrix4(float m_[4][4]) {
  //  std::copy_n(&m_[0][0], 16, this->m[0].begin());
  for (int col = 0; col < 4; col++)
    for (int row = 0; row < 4; row++) {
      // m[3][2] the 4'th column and 3'th row
      // = m_[2][3]
      m[col][row] = m_[row][col];
    }
}

Matrix4 &Matrix4::operator=(const Matrix4 &other) = default;

bool Matrix4::operator==(const Matrix4 &other) const { return m == other.m; }

bool Matrix4::operator!=(const Matrix4 &other) const {
  return !(this->m == other.m);
}

Matrix4::Matrix4(float row00, float row01, float row02, float row03,
                 float row10, float row11, float row12, float row13,
                 float row20, float row21, float row22, float row23,
                 float row30, float row31, float row32, float row33) {
  m[0][0] = row00;
  m[1][0] = row01;
  m[2][0] = row02;
  m[3][0] = row03;

  m[0][1] = row10;
  m[1][1] = row11;
  m[2][1] = row12;
  m[3][1] = row13;

  m[0][2] = row20;
  m[1][2] = row21;
  m[2][2] = row22;
  m[3][2] = row23;

  m[0][3] = row30;
  m[1][3] = row31;
  m[2][3] = row32;
  m[3][3] = row33;
}

Matrix4 Matrix4::Inverse(const Matrix4 &mat1) { return {glm::inverse(mat1.m)}; }

Matrix4::Matrix4(std::array<std::array<float, 4>, 4> m_) {
  for (int col = 0; col < 4; col++)
    for (int row = 0; row < 4; row++) {
      // m[3][2] the 4'th column and 3'th row
      // = m_[2][3]
      m[col][row] = m_[row][col];
    }
}

Matrix4 Matrix4::Transpose(const Matrix4 &mat) { return glm::transpose(mat.m); }

Matrix4 Matrix4::Multiply(const Matrix4 &lhs, const Matrix4 &rhs) {
  return {lhs.m * rhs.m};
}
std::ostream &operator<<(std::ostream &os, const Matrix4 &mat4) {
  os << "[ " << mat4.m << " ]" << std::endl;
  return os;
}

#endif
size_t std::hash<DR::Matrix4>::operator()(const DR::Matrix4 &mat) {
  // clang-format off
  std::string mat_str = fmt::format(
      "{:.4f},{:.4f},{:.4f},{:.4f},"
               "{:.4f},{:.4f},{:.4f},{:.4f},"
               "{:.4f},{:.4f},{:.4f},{:.4f},"
               "{:.4f},{:.4f},{:.4f},{:.4f}",
              mat[0][0], mat[0][1], mat[0][2], mat[0][3],
              mat[1][0], mat[1][1], mat[1][2], mat[1][3],
              mat[2][0], mat[2][1], mat[2][2], mat[2][3],
              mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
  // clang-format on
  return std::hash<std::string>{}(mat_str);
}

Matrix4 DR::Matrix4::Identity() {
  return mat4::Identity_; 
}
