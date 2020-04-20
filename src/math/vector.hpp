#ifndef DI_VECTOR_HPP
#define DI_VECTOR_HPP
#include <iostream>
#include <math/math_utils.hpp>
#include <utils/di_global.h>
NAMESPACE_BEGIN(DR)

class Vector2f;
class Vector3f;
class Point2f;
class Point2f;
class Normal3f;
class Normal3f;

class Vector2f {
public:
  union {
    struct {
      float x, y;
    };
    struct {
      float u, v;
    };
  };

public:
  inline Vector2f() : x(0), y(0) {}
  inline Vector2f(float xx, float yy) : x(xx), y(yy) {}
  inline Vector2f(float v):x(v),y(v){}
  inline Vector2f(const Vector2f &other) : x(other.x), y(other.y) {}
  inline Vector2f &operator=(const Vector2f &other) {
    x = other.x;
    y = other.y;
    return *this;
  }
  inline bool has_nan() const {
    return (std::isnan(x)) || (std::isnan(y));
  }

  // operator
  // immutable / const

  inline bool operator==(const Vector2f &other) const {
    return (x == other.x) && (y == other.y);
  }

  inline bool operator!=(const Vector2f &other) const {
    return !((*this) == other);
  }
  inline Vector2f operator-() const { return Vector2f{-x, -y}; }
  inline Vector2f operator+(const Vector2f &other) const {
    return Vector2f{x + other.x, y + other.y};
  }
  inline Vector2f operator-(const Vector2f &other) const {
    return Vector2f{x - other.x, y - other.y};
  }
  inline Vector2f operator*(float other) const {
    return Vector2f(x * other, y * other);
  }
  inline Vector2f operator/(float other) const {
    float inv = 1.0f / other;
    return Vector2f(x * inv, y * inv);
  }

  inline friend Vector2f operator*(const float &other, const Vector2f &v) {
    return Vector2f(v.x * other, v.y * other);
  }

  inline friend Vector2f operator/(const float &other, const Vector2f &v) {
    return Vector2f(other / v.x, other / v.y);
  }

  // mutable

  inline Vector2f &operator+=(const Vector2f &other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  inline Vector2f &operator-=(const Vector2f &other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  inline Vector2f &operator*=(float other) {
    x *= other;
    y *= other;
    return *this;
  }

  inline Vector2f &operator/=(float other) {
    float inv = 1.0f / other;
    x *= inv;
    y *= inv;
    return *this;
  }
  // debug
  inline friend std::ostream &operator<<(std::ostream &os, const Vector2f &v) {
    os << "[ " << v.x << ", " << v.y << " ]";
    return os;
  }

  inline float length() const { return std::sqrt(x * x + y * y); }
  inline float l2_norm() const { return length(); }
  inline float squared_length() const { return (x * x + y * y); }

  float &operator[](int index) {
    // no validation
    return (&x)[index];
  }
  float operator[](int index) const {
    // no validation
    return (&x)[index];
  }

  float at(int index) const {
    if (index < 0 || index > 2) {
      throw std::out_of_range("Vector2f index out of range\n");
    }
    return (&x)[index];
  }

  float &at(int index) {
    if (index < 0 || index > 2) {
      throw std::out_of_range("Vector2 index out of range\n");
    }
    return (&x)[index];
  }

  inline Vector2f normalize() { return (*this) / this->length(); }
};

NAMESPACE_END(DR)

#endif
